/** @file SENSORS_H.h
 *  This file includes sensors for micromouse.
 *  @author Jeremy West
 *  @date   2023-Nov-10
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Motor.h"
#include <Adafruit_VL53L0X.h>
#include "MPU9250.h"

/** @class IR
 *  @brief Class for Infrared (IR) sensors.
 *
 *  This class provides methods for interacting with Infrared sensors.
 */
class IR {
public:
    /** @brief Constructor for the IR class.
     *  
     *  @param pin The pin to which the Infrared sensor is connected.
     */
    IR(int pin);

    /** @brief Get the value from the Infrared sensor.
     *
     *  @return True if the sensor detects an object, false otherwise.
     */
    boolean get();

private:
    int pin_; ///< The pin to which the Infrared sensor is connected.
};

/** @class Time_Of_Flight
 *  @brief Class for Time-of-Flight (ToF) sensors.
 *
 *  This class provides methods for interacting with Time-of-Flight sensors.
 */
class Time_Of_Flight {
public:
    /** @brief Constructor for the Time_Of_Flight class.
     *  
     *  @param address_ The I2C address of the Time-of-Flight sensor (default: 0x29).
     */
    Time_Of_Flight(int address_);

    /** @brief Get the distance measured by the Time-of-Flight sensor.
     *
     *  @return The distance measured by the sensor.
     */
    float get();

    /** @brief Start the Time-of-Flight sensor.
     */
    void start();

    float dist=0; ///< The distance measured by the Time-of-Flight sensor.
    Adafruit_VL53L0X lox; ///< Adafruit VL53L0X sensor object.

private:
    int pin_; ///< The pin to which the Time-of-Flight sensor is connected.
    int address_=0x29; ///< The I2C address of the Time-of-Flight sensor (default: 0x29).
};

#endif
