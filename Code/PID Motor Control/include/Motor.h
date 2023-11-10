/** @file Motor.h
 *  This includes class for generic dc motor control
 *
 */

#ifndef MOTOR__H
#define MOTOR_H

#include <Arduino.h>

/**    Class for motor and its functions
 *  
*/

class Motor {
public:
    Motor(int pwmPin, int dirPin1, int dirPin2);
    void move(int speed, boolean dir);
    void forward(int speed);
    void loop(int speed,boolean dir);
    void reverse(int speed);
    void stop();
    static void TaskWrapper(void* motor);

    int speed;
    bool dir;
private:
    int pwmPin_;
    int dirPin1_;
    int dirPin2_;
    //int encA1_Pin 32;
    //int encA2_Pin 14;
};

#endif
