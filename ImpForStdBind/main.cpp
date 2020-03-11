#include <iostream>
#include <utility>
#include <tuple>

template<int N>
class Arg{};
const Arg<1> _1;
const Arg<2> _2;
const Arg<3> _3;
const Arg<4> _4;
const Arg<5> _5;
const Arg<6> _6;
const Arg<7> _7;
const Arg<8> _8;
const Arg<9> _9;
const Arg<10> _10;
const Arg<11> _11;
const Arg<12> _12;

template<typename Tuple>
class list{
    Tuple params;
public:
    list(Tuple args):params(std::move(args)){}

    template<int N>
    auto operator[](Arg<N> arg){
        return std::get<N-1>(params);
    }

    template<typename T>
    T operator[](T&& t){
        return std::forward<T>(t);
    }

};

template<typename F,typename... ARGS>
class Bind_t{
    F f;
    std::tuple<ARGS...> params;

public:
    explicit Bind_t(F&& f,ARGS&&... args):f(f),params(std::tie(args...)){}

    template <typename... CARGS>
    decltype(auto) operator()(CARGS... args){
        list<std::tuple<CARGS...>> l1(std::tie(args...));
        return call(l1,std::make_index_sequence<sizeof...(ARGS)>());
    }

    template<typename L,unsigned long long... N>
    decltype(auto) call(L&& li,std::index_sequence<N...>){
        return f(std::forward<L>(li)[std::get<N>(params)]...);
    }


};

template<typename F,typename... ARGS>
auto bind(F&& f,ARGS&&... args){
    return Bind_t<F,ARGS...>(std::forward<F>(f),std::forward<ARGS>(args)...);
}


int add(int i,int j,int k,int l){
    std::cout<<"--->"<<i<<" ,"<<j<<" ,"<<k<<" ,"<<l<<std::endl;
    return i+j;
}


int main() {
    auto f = bind(add,_4,_3,_1,_2);
    std::cout<<f(1,2,3,4)<<std::endl;

    return 0;
}
