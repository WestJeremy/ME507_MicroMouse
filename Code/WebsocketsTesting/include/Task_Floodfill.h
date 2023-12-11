/** @file Task_Floodfill.h
 *  This file contains  the header for a task function which uses the
 *  floodfill algorithm to perform maze solving.
 * 
 *  @author Cesar Santana
 *  @date   2023-Nov-22
*/

#ifndef TASK_FLOODFILL_H
#define TASK_FLOODFILL_H

#include <Arduino.h>
#include "Micromouse.h"




class Solver{
public:
Solver(Micromouse* Micromouse);
// This is the function that impliments the maze solving task
void task_floodfill ();

private:
Micromouse* Micromouse_;
};



#endif