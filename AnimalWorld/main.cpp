//
//  main.cpp
//  AnimalWorld
//
//  Created by SunBo on 29/09/2018.
//  Copyright © 2018 孙博. All rights reserved.
//


// This program is written in C++ and with Xcode10, whose ability of header file prompt
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <chrono>
using namespace std;
class Person{
private:
    vector<char> cards{'A','A','A','A','B','B','B','B','C','C','C','C'};
    int numOfStars=3;
    int seed;
public:
    Person(int seed){
        this->seed =(int)chrono::system_clock::now().time_since_epoch().count() + seed;
    }
    
    void removeOneStar(){
        this->numOfStars--;
    }
    void addOneStar(){
        this->numOfStars++;
    }
    char giveOutCard(){
        default_random_engine d;
        d.seed(seed);
        uniform_int_distribution<int> rand(0,static_cast<int>(cards.size()-1));
        auto index = rand(d);
        return *cards.erase(cards.begin()+index);
    }
    int getNumOfStar(){
        return numOfStars;
    }
    int getNumOfCard(){
        return static_cast<int>(cards.size());
    }
    bool isDead(){
        return (numOfStars==0||(cards.size()==0&&numOfStars<3));
    }
    
};

enum class Result{
    Win,Lose,Draw
};

void run(Person& p1, Person& p2){
    bool noWinner = true;
    cout<<"Game start..."<<endl<<"--------------"<<endl;
    
    auto Battle = [](auto a1, auto a2){
        if(a1<a2){
            return a2-a1==2?Result::Win:Result::Lose;
        }
        else if(a1>a2){
            return a1-a2==2?Result::Lose:Result::Win;
        }
        return Result::Draw;
        
    };
    for (int i = 0; i<100; i++) {
        auto cardOfP1 = p1.giveOutCard();
        auto cardOfP2 = p2.giveOutCard();
        if(Battle(cardOfP1,cardOfP2)==Result::Win){
            p1.addOneStar();
            p2.removeOneStar();
        }
        else if(Battle(cardOfP1,cardOfP2)==Result::Lose){
            p1.removeOneStar();
            p2.addOneStar();
        }
        cout<<"Person 1:"<<cardOfP1<<", Star:"<<p1.getNumOfStar()<<",Cards:"<<p1.getNumOfCard()<<endl;
        cout<<"Person 2:"<<cardOfP2<<", Star:"<<p2.getNumOfStar()<<",Cards:"<<p2.getNumOfCard()<<endl;
        if(p1.isDead()||p2.isDead()){
            noWinner = false;
            if(p1.isDead()){
                cout<<"Person 2 Wins"<<endl;
            }
            if(p2.isDead()){
                cout<<"Person 1 Wins"<<endl;
            }

            break;
        }
        else if(p1.getNumOfCard()==0){
            cout<<"Draw"<<endl;
            break;
        }
        else{
            cout<<"Game continues..."<<endl;
        }
        cout<<"--------------"<<endl;
        this_thread::sleep_for(1s);
    }
    if(noWinner){
        cout<<"No Winner"<<endl;
    }
    
}

int main(int argc, const char * argv[]) {
    // This program is a protype for the game Animal World which is inspired by the move Animal World
    while(true){
    Person p1{10},p2{20};
    run(p1,p2);
    }
    return 0;
}
