#include "States.h"


State::State(int limit) : limit_(limit), state_(0){
}

void State::update(){
    
    state_++;
    if (state_ > limit_){
        state_=0;
    }
}

void State::set(int newstate){
    
if (newstate <= limit_){
    state_=newstate;
        
}
else{
            Serial.print("Error: Set state too high.");
    }
}
    
int State::get(){
    return state_;
}

