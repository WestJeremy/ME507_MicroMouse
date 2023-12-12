/** 
 * @file Motor.cpp
 * @brief Implementation of the Motor class.
 * 
 *  @author Jeremy West
 *  @date   2023-Oct-10
 * 
 */


#include "Motor.h"

/**
 * @brief Constructor for the Motor class.
 * 
 * @param pwmPin PWM pin for motor speed control.
 * @param dirPin1 Direction pin 1.
 * @param dirPin2 Direction pin 2.
 * @param encA1Pin Encoder channel A1 pin.
 * @param encA2Pin Encoder channel A2 pin.
 */
Motor::Motor(int pwmPin, int dirPin1, int dirPin2,int encA1Pin,int encA2Pin) : pwmPin_(pwmPin), dirPin1_(dirPin1), dirPin2_(dirPin2), encA1Pin_(encA1Pin), encA2Pin_(encA2Pin){
    pinMode(pwmPin_, OUTPUT);
    pinMode(dirPin1_, OUTPUT);
    pinMode(dirPin2_, OUTPUT);
    pinMode(encA1Pin_, INPUT);
    pinMode(encA2Pin_, INPUT);
    //pinMode(encA2_Pin, INPUT);
    analogWrite(pwmPin_, 0);  // Initialize PWM with 0% duty cycle
}


/**
 * @brief Update the motor encoder position.
 */
void Motor:: updateEncoder() {
    //Print statements in this function will crash the microcontroller due to ISR so dont do that
        int8_t shaft_angle = 0.0;
        int MSB = digitalRead(encA1Pin_);
        int LSB = digitalRead(encA2Pin_);

        int encoded = (MSB << 1) | LSB;
        int sum = (lastEncoded << 2) | encoded;

        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
            encoderPosition-- ;
        } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
            encoderPosition++ ;
        }

        lastEncoded = encoded;
}

/**
 * @brief Zero the motor encoder position.
 */
void Motor::zeroEncPos(){
    int zero=0;
    encoderPosition=zero;
} 


/**
 * @brief Move the motor at a specified speed.
 * 
 * @param speed Speed of the motor.
 */
void Motor::move(int speed) {
    boolean dir=(speed >= 0) ? 1 : 0; //if speed in negative set dir in reverse
    uint8_t abs_speed = speed;
    digitalWrite(dirPin1_, dir);
    digitalWrite(dirPin2_, !dir);
    analogWrite(pwmPin_, abs_speed);  // Set the PWM duty cycle (0-255)

}

/**
 * @brief Move the motor at a specified speed and direction.
 * 
 * @param speed Speed of the motor.
 * @param dir Direction of the motor.
 */
void Motor::move(int speed,boolean dir) {
    digitalWrite(dirPin1_, dir);
    digitalWrite(dirPin2_, !dir);
    analogWrite(pwmPin_, speed);  // Set the PWM duty cycle (0-255)
    vTaskDelay(10);
}

/**
 * @brief Set the goal position for the motor in encoder counts.
 * 
 * @param pos The goal position.
 */
void Motor:: setgoalpos(int pos) {
goalpos=pos;
}

/**
 * @brief Move the motor to a specified position with a specified speed.
 * 
 * @param goalpos The goal position.
 * @param speed Speed of the motor.
 */
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

/**
 * @brief Move the motor to a specified position using PID control.
 */
void Motor::setposPID() {

    int t_step=10;

    int error=goalpos-encoderPosition;
    float pre_error=0;
    float I_error=0;
    float dt_error=0;
    float d_error;

    int p_in;
    int i_in;
    int d_in;

    float kp = 1;
    float ki=0.01;
    float kd=0.01;

    float t_in;

    for(;;){

        error=goalpos-encoderPosition;
        I_error=I_error+error;
        d_error=error-pre_error/t_step;

        p_in=error*kp;
        i_in=I_error*ki;
        d_in=-d_error*kd;

        t_in=p_in+i_in+d_in;
        t_in=(t_in > 255) ? 255 : t_in; //if the controler input is oversaturated write it to 100%
        move(t_in);

        vTaskDelay(t_step);
    }
}

/**
 * @brief Test function for moving the motor forward and backward.
 * 
 * @param speed Speed of the motor.
 */
void Motor::Movetest(int speed) {
    move(speed);
    for(;;){
        move(speed);

        
        vTaskDelay(1000);
        move(-speed*2); 
        vTaskDelay(2000); 

    }
}

/**
 * @brief Perform a stepwise movement to a specified position using PID control.
 */
void Motor::stepsetposPID() {

    int t_step=10;

    int error=goalpos-encoderPosition;
    float pre_error=0;
    float I_error=0;
    float dt_error=0;
    float d_error;

    int p_in;
    int i_in;
    int d_in;

    float kp = 1;
    float ki=0.01;
    float kd=0.01;

    float t_in;

    

        error=goalpos-encoderPosition;
        I_error=I_error+error;
        d_error=error-pre_error/t_step;

        p_in=error*kp;
        i_in=I_error*ki;
        d_in=-d_error*kd;

        t_in=p_in+i_in+d_in;
        t_in=(t_in > 255) ? 255 : t_in; //if the controler input is oversaturated write it to 100%
        move(t_in);

        vTaskDelay(t_step);
    
}

/**
 * @brief Perform a stepwise movement to a specified position using PID control.
 * 
 * @param error Error value.
 */
void Motor::stepsetposPID(float error) {

    int t_step=10;


    float pre_error=0;
    float I_error=0;
    float dt_error=0;
    float d_error;

    int p_in;
    int i_in;
    int d_in;

    float kp = 1;
    float ki=0.01;
    float kd=0.01;

    float t_in;

    

        error=goalpos-encoderPosition;
        I_error=I_error+error;
        d_error=error-pre_error/t_step;

        p_in=error*kp;
        i_in=I_error*ki;
        d_in=-d_error*kd;

        t_in=p_in+i_in+d_in;
        t_in=(t_in > 255) ? 255 : t_in; //if the controler input is oversaturated write it to 100%
        move(t_in);

        vTaskDelay(t_step);
    
}

/**
 * @brief Perform a stepwise movement to a specified position using PID control.
 * 
 * @param U Control input.
 */
void Motor::stepsetposPIDU(float U) {
    int t_step=10;

    float pre_error=0;
    float I_error=0;
    float dt_error=0;
    float d_error=0;

    int p_in=0;
    int i_in=0;
    int d_in=0;

    float kp = 1;
    float ki=0.01;
    float kd=0.01;

    float t_in;

    //float error=goalpos-encoderPosition+U;

    float error=goalpos-encoderPosition;

    //float error=U;
    I_error=I_error+error;
    d_error=error-pre_error/t_step;

    p_in=error*kp;
    i_in=I_error*ki;
    d_in=-d_error*kd;

    t_in=p_in+i_in+d_in;
    float tu=kp*U;//+ki*U+kd*U;

/*     Serial.print("pos: ");
    Serial.print(p_in+i_in+d_in);
    Serial.print("      ");
 */
 

    t_in=(t_in > 200) ? 200 : t_in; //if the controler input is oversaturated write it to 100%
    t_in=t_in+tu;
    t_in=(t_in > 255) ? 255 : t_in; //if the controler input is oversaturated write it to 100%
    move(t_in);
    //Serial.print("t_in: ");
    //Serial.println(t_in);

    vTaskDelay(t_step);
}

/**
 * @brief Perform a a move forward at a constant speed
 * 
 * @param speed speed 0-255.
 */
void Motor::forward(int speed) {
    digitalWrite(dirPin1_, HIGH);
    digitalWrite(dirPin2_, LOW);
    analogWrite(pwmPin_, speed);  // Use analogWrite to set the PWM duty cycle (0-1023)
}

/**
 * @brief Test loop for speed and direction
 * 
 * @param speed speed 0-255.
 * @param dir boolean
 */
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

/**
 * @brief Reverse motor
 * 
 * @param speed speed 0-255.
 * 
 */
void Motor::reverse(int speed) {
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, HIGH);
    analogWrite(pwmPin_, speed);
}

/**
 * @brief get encoder position
 * 
 */
boolean Motor::getenc() {
    boolean enc_read=digitalRead(encA1Pin_);
    return enc_read;
}

/**
 * @brief stop motor
 * 
 */
void Motor::stop() {
    analogWrite(pwmPin_, 0);  // Stop by setting PWM to 0
    digitalWrite(dirPin1_, LOW);
    digitalWrite(dirPin2_, LOW);
}

