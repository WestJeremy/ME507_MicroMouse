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
            encoderPosition++ ;
        } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
            encoderPosition-- ;
        }

        lastEncoded = encoded;

        // Print the current position for debugging
        //shaft_angle=encoderPosition/360;
        //Serial.print("Encoder Position: ");
        //Serial.println(encoderPosition);
}

//Encoder update loop 
void Motor::ISR_Enc_Read() {

  //attachInterrupt(encA1Pin_, &Motor::updateEncoder, CHANGE);
  //attachInterrupt(encA2Pin_, updateEncoder, CHANGE);

  for(;;){
    vTaskDelay(100);
  }
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

void Motor:: setgoalpos(int pos) {
goalpos=pos;
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

    float kp = 2;
    float ki=0.0;
    float kd=-0.1;

    float t_in;

    for(;;){

        error=goalpos-encoderPosition;
        I_error=I_error+error;
        d_error=error-pre_error/t_step;

        p_in=error*kp;
        i_in=I_error*ki;
        d_in=d_error*kd;

        t_in=p_in+i_in+d_in;
        t_in=(t_in > 50) ? 50 : t_in; //if the controler input is oversaturated write it to 100%
        move(t_in);

        if (error!= 0){
/*             Serial.print("Goal Pos: ");
            Serial.println(goalpos);
            Serial.print("Kp: ");
            Serial.println(kp);
            Serial.print("Error: ");
            Serial.println(error);
            Serial.print("P input: ");
            Serial.println(p_in);
            Serial.print("I input: ");
            Serial.println(i_in);
            Serial.print("D input: ");
            Serial.println(d_in);
            Serial.print("Total input: ");
            Serial.println(t_in); */
        }
        vTaskDelay(t_step);
        
    }
}


void Motor::Movetest(int speed) {
    move(speed);
    for(;;){
        move(speed);

        
        vTaskDelay(1000);
        move(-speed*2); 
        vTaskDelay(2000); 

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

