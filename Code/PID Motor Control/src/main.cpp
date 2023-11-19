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

#define ENCAPin 32
#define ENCBPin 14
boolean x=1;
uint8_t pos=0;
uint8_t curpos=0;
uint8_t PWMI1=255 ;
uint8_t PWMI2=126 ;

Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin,ENCAPin, ENCBPin ); 

// ___________________Functions___________________
void Baitandswitch(void* parameter){

  motor1.Movetest(100);
}


//Encoder update loop 
void enc_loop(void* parameter){

  for(;;){
    motor1.updateEncoder();
    vTaskDelay(1);
  }
}



void setpostask(void* parameter){
  
  //motor1.goalpos=100;
  //motor1.setpos(100,60);
  motor1.setgoalpos(100);
  motor1.setposPID();

}



//___________________Setup and Loop Functions___________________

void setup() {
  Serial.begin(115200);

  //motor1.move(100);
  //boolean E = motor1.getenc();
  //Serial.print(E);
  //xTaskCreate (Baitandswitch, "A",  4096, NULL, 1, NULL);
  xTaskCreate (enc_loop, "ENC",  4096, NULL, 2, NULL);
  xTaskCreate (setpostask, "setpos",  4096, NULL, 1, NULL);


}
 
void loop() {
  delay(1000);
}  