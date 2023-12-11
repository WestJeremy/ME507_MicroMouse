/** @file States.h
 *  This includes class for generic dc motor control
 *
 */

#ifndef STATES_H
#define STATES_H

#include <Arduino.h>

/**    States
 *  
*/


class State {
public:
    State(int limit);
    void update();
    void set(int newstate);
    int get();


private:
    volatile int state_;
    int limit_;
};

#endif
