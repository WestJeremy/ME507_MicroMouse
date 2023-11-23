/** @file Task_Sensor.cpp
 *  This file contains a task function which recieves sensor data
 *  and then updates a set of shared variables that will be used to
 *  update wall locations for a maze solving algorithm.
 * 
 *  @author Cesar Santana
 *  @date 2023-Nov-22
*/

#include <Arduino.h>
#include "taskqueue.h"
#include "taskshare.h"
#include "shares.h"
#include "Task_Sensor.h"

void task_sensor (void* p_params)
{
    const uint8_t R_IR = 9;
    const uint8_t L_IR = 8;

    for(;;)
    {
        // These two if statements check the output of R_IR and update a shared variable
        if (digitalRead(R_IR) == true)
        {
            r_wall.put(1);
        }
        if (digitalRead(R_IR) == false)
        {
            r_wall.put(0);
        }

        // These two if statements check the output of L_IR and update a shared variable
        if (digitalRead(L_IR) == true)
        {
            l_wall.put(1);
        }
        if (digitalRead(L_IR) == false)
        {
            l_wall.put(0);
        }

        // include TOF detection here
    }

}