#include <Arduino.h>
#include "Motor.h"
 /*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Adapted for esp32 feather by Jeremy West 2023
  Home
*/

#define PWM1_pin 15
#define AIN1_pin 33
#define AIN2_pin 27
#define PWM2_pin 4
#define BIN1_pin 5
#define BIN2_pin 18

#define ENCA 32
#define ENCB 14
boolean x=1;
uint8_t pos=0;
uint8_t curpos=0;
uint8_t PWMI1=255 ;
uint8_t PWMI2=126 ;



//Motor(int pwmPin, int dirPin1, int dirPin2);

 
/**
void MotorPID(void *pvParameters){
  SquareWaveParams* params = (SquareWaveParams*)pvParameters;
  const int pin = params->pin;
  int frequency = params->frequency;
  int duty_cycle = params->duty_cycle;

  float time_on;
  float time_off;

  
    while(true){
      if (curpos>pos){

      digitalWrite(pin,1);
      vTaskDelay(time_on);
      digitalWrite(pin,0);
      vTaskDelay(time_off);

    }
    Serial.print("Error: exited infinite loop.");
    }
  else{
    Serial.print("Please input duty cycle between 0 and 100");
    
  }
}



  motor1.move(200,1);
  delay(1000);
  motor1.move(200,0);
*/


void Baitandswitch(void* parameter){
  Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin );  // Replace with actual pin numbers
  motor1.loop(200,1);

}

void Baitandswitch(void* parameter){
  Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin );  // Replace with actual pin numbers

  int8_t goalpos=0; 
  int8_t curpos=0; 
  

  motor1.loop(200,1);

}


void setup() {
  Serial.begin(115200);
  Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin );  // Replace with actual pin numbers

  motor1.speed = 200;
  motor1.dir = 1;


  xTaskCreate (Baitandswitch, "A",  4096, NULL, 1, NULL);
  //xTaskCreate (Square_Wave, "Slow",  4096, &SlowTask, 2, NULL);
}
 
void loop() {
  delay(1000);
}