/** @file shares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  @author Cesar Santana
 *  @date   2023-Nov-22
 */

#ifndef SHARES_H
#define SHARES_H

#include "Printstream.h"
#include "taskqueue.h"
#include "taskshare.h"

// A share which hold thecurrent orientation of the micromouse
extern Share<uint8_t> direction;
// A share which holds whether the right IR sensor detects a wall or not
extern Share<char> r_wall;

// A share which holds whether the left IR sensor detects a wall or not
extern Share<char> l_wall;

// A share which holds whether the front TOF sensor detects a wall or not
extern Share<char> f_wall;



#endif 