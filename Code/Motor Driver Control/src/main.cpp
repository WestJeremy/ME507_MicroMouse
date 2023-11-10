#include <Arduino.h>


 /*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Adapted for esp32 feather by Jeremy West 2023
  Home

*/

#define PWM1 15
#define AIN1 33
#define AIN2 27
#define PWM2 4
#define BIN1 5
#define BIN2 18

#define ENCA 32
#define ENCB 14

uint8_t PWMI1=255 ;
uint8_t PWMI2=126 ;

int out;

void setup() {
  Serial.begin(115200);
  pinMode(PWM1,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  Serial.print("Pins configured");


    
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);

  Serial.print("motor direction configured");

  //pot=analogRead(A0);
  //out=map(pot,0,1023,0,255);
  
  analogWrite(PWM1,PWMI1); //Speed control of Motor A
  analogWrite(PWM2,PWMI2); //Speed control of Motor B
  //Serial.print("good things can happen");
  
  Serial.print("motor speed configured");
  
}
 
void loop() {

digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
digitalWrite(AIN2,LOW);

analogWrite(PWM1,PWMI1); //Speed control of Motor A

delay(200);

digitalWrite(AIN1,LOW); //Motor A Rotate Clockwise
digitalWrite(AIN2,HIGH);

analogWrite(PWM1,PWMI1/2); //Speed control of Motor A

Serial.print(digitalRead(ENCA));
delay(200);

}