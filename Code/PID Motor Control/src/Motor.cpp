#include "Motor.h"
//#include <wiringPi.h>

Motor::Motor(int pwmPin, int dirPin1, int dirPin2,int encA1Pin,int encA2Pin) : pwmPin_(pwmPin), dirPin1_(dirPin1), dirPin2_(dirPin2), encA1Pin_(encA1Pin), encA2Pin_(encA2Pin){
    pinMode(pwmPin_, OUTPUT);
    pinMode(dirPin1_, OUTPUT);
    pinMode(dirPin2_, OUTPUT);
    pinMode(encA1Pin_, INPUT);
    pinMode(encA2Pin_, INPUT);
    //pinMode(encA2_Pin, INPUT);
    analogWrite(pwmPin_, 0);  // Initialize PWM with 0% duty cycle
}


void Motor:: updateEncoder() {
        int8_t shaft_angle = 0.0;
        int MSB = digitalRead(encA1Pin_);
        int LSB = digitalRead(encA2Pin_);

        int encoded = (MSB << 1) | LSB;
        int sum = (lastEncoded << 2) | encoded;

        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
            encoderPosition++;
        } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
            encoderPosition--;
        }

        lastEncoded = encoded;

        // Print the current position for debugging
        //shaft_angle=encoderPosition/360;
        Serial.println(encoderPosition);
}

void Motor::move(int speed) {
    boolean dir=(speed >= 0) ? 1 : 0; //if speed in negative set dir in reverse
    uint8_t abs_speed = speed;
    digitalWrite(dirPin1_, dir);
    digitalWrite(dirPin2_, !dir);
    analogWrite(pwmPin_, abs_speed);  // Set the PWM duty cycle (0-1023)
}

void Motor::move(int speed,boolean dir) {
    digitalWrite(dirPin1_, dir);
    digitalWrite(dirPin2_, !dir);
    analogWrite(pwmPin_, speed);  // Set the PWM duty cycle (0-1023)
    vTaskDelay(10);
}

void Motor::setpos(int goalpos, int speed) {
    boolean dirt=1;
    for(;;){
        updateEncoder();
        if (goalpos>encoderPosition){
            dirt=1;
        } else if(goalpos<encoderPosition){
            dirt=0;
        }
        move(speed,dirt);
        vTaskDelay(1);
        Serial.println(encoderPosition);
    }
}

void Motor::setposPID(int goalpos) {
    int8_t error=goalpos-encoderPosition;
    int8_t kp = 4;
    int8_t speedy;
    for(;;){
        updateEncoder();
        if (error= 0){
        Serial.print("Goal Pos: ");
        Serial.println(goalpos);
        Serial.print("Pos: ");
        Serial.println(encoderPosition);
        Serial.print("Speedy:");
        Serial.println(speedy);
        Serial.print("error: ");
        Serial.println(error);
        }
        error=goalpos-encoderPosition;
        speedy=error*kp;
        //speedy=error;


        //Serial.println(100*kp);
        //Serial.println(error*kp);
        //Serial.println(speedy);

        move(error*kp);
        vTaskDelay(1);
        
    }
}


void Motor::Movetest(int speed) {
    move(speed);
    for(;;){
        updateEncoder();
        
        vTaskDelay(1);  
    }
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
        Serial.print(dir);
        //move(speed,!dir);
        vTaskDelay(1000);
        Serial.print(!dir);
    }
}

void Motor::reverse(int speed) {
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, HIGH);
    analogWrite(pwmPin_, speed);
}

boolean Motor::getenc() {
    boolean enc_read=digitalRead(encA1Pin_);
    return enc_read;
}

void Motor::stop() {
    analogWrite(pwmPin_, 0);  // Stop by setting PWM to 0
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, LOW);
}

