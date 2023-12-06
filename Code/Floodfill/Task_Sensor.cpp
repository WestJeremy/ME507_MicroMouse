/** @file Task_sensor.cpp
 *  This file contains code for a sensor task that will be
 *  used to update the walls of wall arrays
 * 
 *  @author Cesar Santana
 *  @date   2023-Dec-04
*/
#include <Arduino.h>
#include "Printstream.h"
#include "shares.h"
#include "Task_Sensor.h"

/** @brief  Task which takes sensor feedback and uses it to update wall arrays
 *  @param  p_params An unused pointer to nonexistent parameters
*/
void task_sensor (void* p_params)
{
    const int8_t L_IR = 8;      // GPIO pin for left IR sensor
    const int8_t R_IR = 9;      // GPIO pin for right IR sensor
    pinMode(L_IR, INPUT);       // Sets up L_IR as an input
    pinMode(R_IR, INPUT);       // Sets up R_IR as an input
    // need to set up tof sensor here

    for(;;)
    {
        if (direction.get() == 1 || 2 )
        {
            if (digitalRead(L_IR) == false)
            {
                l_wall.put('-');
            }
            if (digitalRead(R_IR) == false)
            {
                r_wall.put('-');
            }
            // include condition for tof reading
            if ()
            {
                f_wall.put('|');
            }
            
        }

        if (direction.get() == 3 || 4)
        {
            if (digitalRead(L_IR) == false)
            {
                l_wall.put('|');
            }
            if (digitalRead(R_IR) == false)
            {
                r_wall.put('|'); 
            }
            // include condition for tof
            if ()
            {
                f_wall.put('-');
            }
        }
        vTaskDelay(5);
    }
}