/** 
 * @file States.h
 * @brief This file includes a class for generic states.
 * @author Jeremy West
 * @date   2023-Dec-10
 */

#ifndef STATES_H
#define STATES_H

#include <Arduino.h>

/** 
 * @class State
 * @brief Class for managing generic states.
 */
class State {
public:
    /**
     * @brief Constructor for the State class.
     * 
     * @param limit Maximum limit for the state values.
     */
    State(int limit);

    /**
     * @brief increment the state by 1.
     */
    void update();

    /**
     * @brief Set the state to a new value.
     * 
     * @param newstate The new state value.
     */
    void set(int newstate);

    /**
     * @brief Get the current state value.
     * 
     * @return The current state value.
     */
    int get();

private:
    volatile int state_; ///< Current state value.
    int limit_;          ///< Maximum limit for the state values.
};

#endif
