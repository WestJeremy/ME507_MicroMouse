/** @file Micromouse.h
 *  This file includes a class for the Jeremy West and Cesar Santana Micromouse project.
 *  @author Jeremy West
 *  @date   2023-Nov-20
 */


#ifndef MICROMOUSE_H
#define MICROMOUSE_H

#include <Arduino.h>
#include "Sensors.h"
#include "States.h"

/** @class Micromouse
 *  @brief Class for the Micromouse project.
 *
 *  This class represents the Micromouse project and provides methods for controlling its behavior.
 */
class Micromouse {
public:
    /** @brief Constructor for the Micromouse class.
     *  
     *  @param motor1 A pointer to the first motor object.
     *  @param motor2 A pointer to the second motor object.
     *  @param IR_L A pointer to the left Infrared (IR) sensor object.
     *  @param IR_R A pointer to the right Infrared (IR) sensor object.
     *  @param TOF A pointer to the Time-of-Flight (ToF) sensor object.
     *  @param IMU A pointer to the MPU9250 Inertial Measurement Unit (IMU) object.
     *  @param state_ A pointer to the State object.
     */
    Micromouse(Motor* motor1, Motor* motor2, IR* IR_L, IR* IR_R, Time_Of_Flight* TOF, MPU9250* IMU, State* state_);

    int IR_L; ///< Left Infrared (IR) sensor value.
    int IR_R; ///< Right Infrared (IR) sensor value.

    int volatile encposL_=0; ///< Left motor encoder position.
    int volatile encposR_=0; ///< Right motor encoder position.
    int position; ///< Current position of the Micromouse.
    
    /** @brief Calibrate the Inertial Measurement Unit (IMU).
     */
    void calIMU();

    /** @brief Move the Micromouse a specified number of cells.
     *
     *  @param Cells The number of cells to move.
     */
    void Move(int Cells);

    /** @brief Perform a stepwise movement using Angular PID control.
     *
     *  @return The angular error after the movement.
     */
    float StepAngularPID();

    /** @brief Move the Micromouse a specified number of cells with stable rotation.
     *
     *  @param Cells The number of cells to move.
     */
    void MoveStable(int Cells);

    float AngError=0; ///< Angular error value.
    float AngZero; ///< Zero angle reference.
    float GoalAngle; ///< Goal angular position.
    float I_error; ///< Integral error for PID control.

    /** @brief Zero the motor encoders.
     */
    void zeroenc();

    /** @brief Stop the movement of the Micromouse.
     */
    void stop();

    /** @brief Perform a stable rotation to a specified angle.
     *
     *  @param angle The target angle for rotation.
     */
    void RotStable(float angle);

    /** @brief Update the motor encoders.
     */
    void updateEncoder();

    State* state_; ///< Pointer to the State object.

    /** @brief Set the state of the Micromouse.
     *
     *  @param NewState The new state to set for the Micromouse.
     */
    void setstate(int NewState);

private:
    Motor* motor1_; ///< Pointer to the first motor object.
    Motor* motor2_; ///< Pointer to the second motor object.

    IR* IR_L_; ///< Pointer to the left Infrared (IR) sensor object.
    IR* IR_R_; ///< Pointer to the right Infrared (IR) sensor object.

    Time_Of_Flight* TOF_; ///< Pointer to the Time-of-Flight (ToF) sensor object.
    MPU9250* IMU_; ///< Pointer to the MPU9250 Inertial Measurement Unit (IMU) object.
};

#endif
