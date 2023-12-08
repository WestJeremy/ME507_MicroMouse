/** @file SENSORS_H.h
 *  This includes class for Jeremy West and Cesar Santana Micromouse project
 *
 */



#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Motor.h"
#include <Adafruit_VL53L0X.h>
#include "MPU9250.h"

/**    Micromouse
 *  
*/

class IR {
public:
    IR(int pin);
    boolean get();

private:
    int pin_;
};

class Time_Of_Flight {
public:
    Time_Of_Flight(int address_);
    float get();
    void start();
    float dist=0;
    Adafruit_VL53L0X lox;


private:
    int pin_;
    int address_=0x29;

};



#endif