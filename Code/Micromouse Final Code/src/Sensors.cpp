/** 
 * @file Sensors.cpp
 * @brief Implementation of the Sensors class.
 *  @author Jeremy West
 *  @date   2023-Nov-10
 */

#include "Sensors.h"

/**
 * @brief Constructor for the IR class.
 * 
 * @param pin The pin connected to the IR sensor.
 */
IR::IR(int pin):pin_(pin){
    pinMode(pin, INPUT);
}

/**
 * @brief Get the status of the IR sensor.
 * 
 * @return True if an object is detected, false otherwise.
 */
boolean IR::get() {
    //read if object detected 1 yes 0 no
    return(!digitalRead(pin_));
}

/**
 * @brief Constructor for the Time_Of_Flight class.
 * 
 * @param address The I2C address of the TOF sensor.
 */
Time_Of_Flight::Time_Of_Flight(int address): address_(address){
    Serial.println("here we go...");

}


/**
 * @brief Initialize the TOF sensor.
 */
void Time_Of_Flight::start(){
// wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }


  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(address_)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

/**
 * @brief Get the distance measured by the TOF sensor.
 * 
 * @return The distance in millimeters.
 */
float Time_Of_Flight::get() {
  //for TOF sensor https://electronoobs.com/eng_arduino_tut73.php
  VL53L0X_RangingMeasurementData_t measure;
    
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    dist=measure.RangeMilliMeter;
  } else {
    Serial.println(" out of range ");
    dist=8000;
  }

    return dist;
}