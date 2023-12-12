/** @file Micromouse.h
 *  This includes class for Jeremy West and Cesar Santana Micromouse project
 *
 */



#ifndef MICROMOUSE_H
#define MICROMOUSE_H

#include <Arduino.h>
#include "Sensors.h"

/**    Micromouse
 *  
*/


class Micromouse {
public:
    Micromouse(Motor* motor1,Motor* motor2,IR* IR_L,IR* IR_R,Time_Of_Flight* TOF,MPU9250* IMU);

    int IR_L;
    int IR_R;


    int encposL;
    int encposR;
    int position;
    void calIMU();
    void Move(int Cells);
    float StepAngularPID();
    void MoveStable(int Cells);
    float AngError=0;
    float AngZero;
    float GoalAngle;
    float I_error;
    void zeroenc();
    void stop();
    void RotStable(float angle);
    
    



private:
    Motor* motor1_;
    Motor* motor2_;

    IR* IR_L_;
    IR* IR_R_;
    Time_Of_Flight* TOF_;
    MPU9250* IMU_;

};

#endif