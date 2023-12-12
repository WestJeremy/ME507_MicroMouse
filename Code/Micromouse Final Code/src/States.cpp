/** 
 * @file States.cpp
 * @brief Implementation of the State class.
 * @author Jeremy West
 * @date   2023-Dec-10
 */
#include "States.h"

/**
 * @brief Constructor for the State class.
 * 
 * @param limit The maximum value for the state.
 */
State::State(int limit) : limit_(limit), state_(0){
}

/**
 * @brief Increment the state. If the state surpasses the limit, reset to 0.
 */
void State::update(){
    
    state_++;
    if (state_ > limit_){
        state_=0;
    }
}

/**
 * @brief Set the state to a new value.
 * 
 * @param newstate The new state value.
 */
void State::set(int newstate){
    
if (newstate <= limit_){
    state_=newstate;
        
}
else{
            Serial.print("Error: Set state too high.");
    }
}

/**
 * @brief Get the current state.
 * 
 * @return The current state value.
 */
int State::get(){
    return state_;
}

