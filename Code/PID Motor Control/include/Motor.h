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
    Motor(int pwmPin, int dirPin1, int dirPin2,int encA1Pin,int encA2Pin);
    void move(int speed, boolean dir);
    void move(int speed);
    void forward(int speed);
    void loop(int speed,boolean dir);
    void reverse(int speed);
    boolean getenc();
    void updateEncoder();
    void stop();
    void setpos(int goalpos, int speed);
    void setposPID(int goalpos);
    void Movetest(int speed);

    int speed; 
    bool dir;
    int8_t goalpos=0; 
    int8_t curpos=0; 

private:
    int pwmPin_;
    int dirPin1_;
    int dirPin2_;
    int encA1Pin_;
    int encA2Pin_;
    volatile int encoderPosition;
    volatile int lastEncoded;
    volatile long lastMillis;
};

#endif
