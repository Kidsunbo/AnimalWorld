#include <iostream>
#include <tuple>
using namespace std;


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
const Arg<13> _13;
const Arg<14> _14;
const Arg<15> _15;
const Arg<16> _16;
const Arg<17> _17;
const Arg<18> _18;
const Arg<19> _19;
const Arg<20> _20;



template<typename... ARGS>
class list{
    std::tuple<ARGS...> params;

public:
    list(ARGS... args):params(std::tie(args...)){}

    template<int N>
    decltype(auto) operator[](const Arg<N> arg){
        return std::get<N-1>(params);
    }

    template<typename T>
    decltype(auto) operator[](T&& t){
        return std::forward<T>(t);
    }

};



template <typename F,typename... ARGS>
class Bind_t{
    F f;
    std::tuple<ARGS...> params;
public:
    Bind_t(F&& f,ARGS&&... args):f(f),params(std::tie(args...)){}

    template<typename... CARGS>
    decltype(auto) operator()(CARGS... cargs){
        list<CARGS...> li(cargs...);
        return call(li,std::make_index_sequence<sizeof...(ARGS)>());
    }

    template<typename L,std::size_t... N>
    decltype(auto) call(L&& l,std::index_sequence<N...>){
        return f(l[std::get<N>(params)]...);
    }
};

template<typename F,typename... ARGS>
decltype(auto) bind(F&& f,ARGS&&... args){
    return Bind_t<F,ARGS...>(std::forward<F>(f),std::forward<ARGS>(args)...);
}

int sum(int a1,int a2,int a3,int a4){
    std::cout<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<endl;
    return a1+a2+a3+a4;
}


int main(){

    auto f = bind(sum,_4,_3,_1,_2);
    std::cout<<f(1,2,3,4)<<std::endl;


    return 0;
}