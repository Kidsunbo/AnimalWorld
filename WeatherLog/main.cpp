#include <cpr/cpr.h>
#include <filesystem>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <tuple>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
using std::chrono_literals::operator ""s;
using std::string_literals::operator ""s;


auto getConfig() {
	// Check for existing
	if (!std::filesystem::exists("./config.json")) {
		std::fstream f("./config.json",std::ios::out);
		nlohmann::json j;
		j["key"] = "e3885fceebcc8f54df24b87b8c0b61a1";
		j["use_ip"] = false;
		j["position"] = "110108";
		f << j.dump(4);
		f.flush(); // Close with destructor, which is not a good behavior.
	}
	std::fstream f("./config.json",std::ios::in);
	std::stringstream json_buf;
	json_buf << f.rdbuf();
	auto j = nlohmann::json::parse(json_buf.str());
	
	if (j.contains("key") && j.contains("use_ip") && j.contains("position"))
		return std::make_tuple(j["key"].get<std::string>(), j["use_ip"].get<bool>(), j["position"].get<std::string>(), true);
	else return std::make_tuple(j["key"].get<std::string>(), j["use_ip"].get<bool>(), j["position"].get<std::string>(), false);
}

auto getPositionWithIP(std::string key) {
	auto response = cpr::Get(cpr::Url("https://restapi.amap.com/v3/ip"), cpr::Parameters{{"key",key}});
	if (response.status_code != 200) return std::make_tuple(""s, false);
	auto j = nlohmann::json::parse(response.text);
	if (!j.contains("status") || !j.contains("adcode")) return std::make_tuple(""s, false);
	else if (j["status"].get<std::string>() != "1") return std::make_tuple(""s, false);
	else return std::make_tuple(j["adcode"].get<std::string>(), true);
}

auto getDateTime() {
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto time = std::localtime(&now);
	std::stringstream s;
	s << std::put_time(time, "%Y年%m月%d日%H:%M:%S");
	return s.str();
}

int main() {

	while (true) {

		auto config = getConfig();
		if (!std::get<3>(config)) {
			std::this_thread::sleep_for(60s);
			continue;
		}

		auto key = std::get<0>(config);
		auto use_ip = std::get<1>(config);
		auto position = std::get<2>(config);


		if (use_ip) {
			auto res = getPositionWithIP(key);
			if (std::get<1>(res)) position = std::get<0>(res);
		}

		auto response = cpr::Get(cpr::Url("https://restapi.amap.com/v3/weather/weatherInfo"), cpr::Parameters{
			{"key",key},
			{"city",position},
			{"extensions","base"}});

		if (response.status_code != 200) {
			std::this_thread::sleep_for(60s);
			continue;
		}
		auto j = nlohmann::json::parse(response.text);
		if (!j.contains("status") || !j.contains("info") || !j.contains("lives")|| j["lives"].size() == 0) {
			std::this_thread::sleep_for(60s);
			continue;
		}
		if (j["status"].get<std::string>() != "1") {
			std::this_thread::sleep_for(60s);
			continue;
		}
		auto info = j["lives"][0];
		std::fstream log("./weather.log", std::ios::app);
		log << info["city"].get<std::string>() << "在" << getDateTime() << "温度为" << info["temperature"].get<std::string>() << "℃，" << info["weather"].get<std::string>() << "，湿度" << info["humidity"].get<std::string>()
		<< "，风向" << info["winddirection"].get<std::string>() << "，风力" << info["windpower"].get<std::string>() << std::endl;
		log.flush();
		std::this_thread::sleep_for(600s);
	}
	return 0;
}