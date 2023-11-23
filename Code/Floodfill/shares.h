/** @file shares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  @author Cesar Santana
 *  @date   2023-Nov-22
 */

#ifndef _SHARES_H_
#define _SHARES_H_

#include "taskqueue.h"
#include "taskshare.h"

// A share which holds whether the right IR sensor detects a wall or not
extern Share<uint8_t> r_wall;

// A share which holds whether the left IR sensor detects a wall or not
extern Share<uint8_t> l_wall;

// A share which holds whether the front TOF sensor detects a wall or not
extern Share<uint8_t> f_wall;



#endif _SHARES_H_