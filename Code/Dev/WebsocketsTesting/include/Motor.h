/** @file Motor.h
 *  This file includes a class for generic DC motor control.
 *
 *   TB6612FNG-Dual-Driver
 *   made on 28 oct 2020
 *   by Amir Mohammad Shojaee @ Electropeak
 *   Adapted for esp32 feather and added functionality by Jeremy West 2023
 * 
 * 
 *  
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

/** @class Motor
 *  @brief Class for controlling a DC motor.
 *
 *  This class provides methods for controlling the movement and position of a DC motor.
 */
class Motor {
public:
    /** @brief Constructor for the Motor class.
     *  
     *  @param pwmPin The PWM pin connected to the motor.
     *  @param dirPin1 The first direction pin of the motor.
     *  @param dirPin2 The second direction pin of the motor.
     *  @param encA1Pin The first encoder pin of the motor.
     *  @param encA2Pin The second encoder pin of the motor.
     */
    Motor(int pwmPin, int dirPin1, int dirPin2, int encA1Pin, int encA2Pin);

    /** @brief Move the motor with specified speed and direction.
     *
     *  @param speed The speed of the motor (0 to 255).
     *  @param dir The direction of the motor (true for forward, false for reverse).
     */
    void move(int speed, boolean dir);

    /** @brief Move the motor with specified speed.
     *
     *  @param speed The speed of the motor (0 to 255).
     */
    void move(int speed);

    /** @brief Move the motor forward with specified speed.
     *
     *  @param speed The speed of the motor (0 to 255).
     */
    void forward(int speed);

    /** @brief Loop function for continuous motor movement with specified speed and direction.
     *
     *  @param speed The speed of the motor (0 to 255).
     *  @param dir The direction of the motor (true for forward, false for reverse).
     */
    void loop(int speed, boolean dir);

    /** @brief Move the motor in reverse with specified speed.
     *
     *  @param speed The speed of the motor (0 to 255).
     */
    void reverse(int speed);

    /** @brief Get the encoder value.
     *
     *  @return True if encoder is active, false otherwise.
     */
    boolean getenc();

    /** @brief Update the motor encoder value.
     */
    void updateEncoder();

    /** @brief Encoder interrupt service routine.
     */
    void ISR_Enc_Read();

    /** @brief Stop the motor.
     */
    void stop();

    /** @brief Reset the encoder position to zero.
     */
    void zeroEncPos();

    /** @brief Set the goal position for the motor.
     *
     *  @param pos The desired goal position.
     */
    void setgoalpos(int pos);

    /** @brief Set the motor position and move to the goal position with specified speed.
     *
     *  @param goalpos The desired goal position.
     *  @param speed The speed of the motor (0 to 255).
     */
    void setpos(int goalpos, int speed);

    /** @brief Set the position using PID control.
     */
    void setposPID();

    /** @brief Set the position using stepwise PID control.
     */
    void stepsetposPID();

    /** @brief Set the position using stepwise PID control with a specified error.
     *
     *  @param error The error value for PID control.
     */
    void stepsetposPID(float error);

    /** @brief Set the position using stepwise PID control with a specified control input.
     *
     *  @param U The control input for PID control.
     */
    void stepsetposPIDU(float U);

    /** @brief Perform a motor movement test with specified speed.
     *
     *  @param speed The speed of the motor (0 to 255).
     */
    void Movetest(int speed);

    int speed; ///< The speed of the motor.
    bool dir; ///< The direction of the motor.
    int goalpos=0; ///< The goal position for the motor.
    int curpos=0; ///< The current position of the motor.
    volatile int encoderPosition; ///< The current encoder position.

private:
    int pwmPin_; ///< The PWM pin connected to the motor.
    int dirPin1_; ///< The first direction pin of the motor.
    int dirPin2_; ///< The second direction pin of the motor.
    int encA1Pin_; ///< The first encoder pin of the motor.
    int encA2Pin_; ///< The second encoder pin of the motor.
    
    volatile int lastEncoded; ///< The last recorded encoder value.
    volatile long lastMillis; ///< The last recorded millisecond value.
};

#endif
