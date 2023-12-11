/** @file Motor.h
 *  This includes class for generic dc motor control
 *
 */

#ifndef MOTOR_H
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
    void ISR_Enc_Read(); 
    void stop();
    void zeroEncPos();

    void setgoalpos(int pos);
    void setpos(int goalpos, int speed);
    void setposPID();
    void stepsetposPID();
    void stepsetposPID(float error);
    void stepsetposPIDU(float U);
    void Movetest(int speed);

    int speed; 
    bool dir;
    int goalpos=0; 
    int curpos=0; 
    volatile int encoderPosition;

private:
    int pwmPin_;
    int dirPin1_;
    int dirPin2_;
    int encA1Pin_;
    int encA2Pin_;
    
    volatile int lastEncoded;
    volatile long lastMillis;
};

#endif
