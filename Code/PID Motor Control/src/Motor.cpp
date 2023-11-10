#include "Motor.h"
//#include <wiringPi.h>

Motor::Motor(int pwmPin, int dirPin1, int dirPin2) : pwmPin_(pwmPin), dirPin1_(dirPin1), dirPin2_(dirPin2) {
    pinMode(pwmPin_, OUTPUT);
    pinMode(dirPin1_, OUTPUT);
    pinMode(dirPin2_, OUTPUT);
    analogWrite(pwmPin_, 0);  // Initialize PWM with 0% duty cycle
}


void Motor::move(int speed,boolean dir) {
    digitalWrite(dirPin1_, dir);
    digitalWrite(dirPin2_, !dir);
    analogWrite(pwmPin_, speed);  // Set the PWM duty cycle (0-1023)
}


void Motor::forward(int speed) {
    digitalWrite(dirPin1_, HIGH);
    digitalWrite(dirPin2_, LOW);
    analogWrite(pwmPin_, speed);  // Use analogWrite to set the PWM duty cycle (0-1023)
}

void Motor::loop(int speed,boolean dir) {
    for(;;){
        move(speed,dir);
        vTaskDelay(1000);
        move(speed,!dir);
        vTaskDelay(1000);
    }
}

void Motor::reverse(int speed) {
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, HIGH);
    analogWrite(pwmPin_, speed);
}

void Motor::stop() {
    analogWrite(pwmPin_, 0);  // Stop by setting PWM to 0
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, LOW);
}

static void TaskWrapper(void* motor) {
    Motor* motorInstance = static_cast<Motor*>(motor);
    if (motorInstance) {
        motorInstance->loop(motorInstance->speed, motorInstance->dir);
    }
    vTaskDelete(NULL); // Clean up the task when it's done
}