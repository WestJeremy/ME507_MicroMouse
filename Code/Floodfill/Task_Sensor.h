/** @file Task_Sensor.h
 *  This file contains the header file for a task function that recieves
 *  sensor data and stores it in a shared variable that is used to update
 *  wall locations in a maze solving algorithm.
 * 
 *  @author Cesar Santana
 *  @date   2023-Nov-22
*/

#ifndef TASK_SENSOR_H
#define TASK_SENSOR_H
#include <Arduino.h>

// This is the function that imppliments the sensor task
void task_sensor (void* p_params);

#endif