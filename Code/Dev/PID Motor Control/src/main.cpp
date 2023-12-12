#include <Arduino.h>
#include "Motor.h"
//#include "FreeRTOS.h"
//#include "task.h"

 /*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Adapted for esp32 feather by Jeremy West 2023
  Home
*/

// Pin Setup
#define button 34

//motor 1
#define PWM1_pin 14
#define AIN1_pin 15
#define AIN2_pin 32

#define ENCAPin 21
#define ENCBPin 4

//motor 2
#define PWM2_pin 12
#define BIN1_pin 33
#define BIN2_pin 27

#define ENC2APin 25
#define ENC2BPin 26

// Global variable shared between ISR and task
volatile bool interruptFlag = false;


boolean x=1;
uint8_t pos=0;
uint8_t curpos=0;


Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin,ENCAPin, ENCBPin ); 
Motor motor2(PWM2_pin, BIN1_pin , BIN2_pin,ENC2APin, ENC2BPin );

//Task Handles
TaskHandle_t myIntTaskHandle = NULL;



// ___________________Functions___________________

void Baitandswitch(void* parameter){
  int speed=100;
 
  motor1.Movetest(speed);
  motor2.Movetest(speed+100);
}

//Encoder update loop 
void enc_loop(void* parameter){

  for(;;){

    motor1.updateEncoder();
    vTaskDelay(1);
  }
}

void bs(){
  //portENTER_CRITICAL_ISR(&mux);
  // Access or modify shared data
  interruptFlag = true;
  Serial.print(interruptFlag);
  motor1.updateEncoder();
  motor2.updateEncoder();
  interruptFlag = false;
  //taskEXIT_CRITICAL();
}

//Update Both Encoders
void EncoderUpdate(){
  motor1.updateEncoder();
  motor2.updateEncoder();
}


void myInterruptTask(void* parameter){

  for(;;){
    vTaskSuspend(NULL); //suspend self
    //Code to run on intterupt goes below
    EncoderUpdate();
   
  }
}


/* interrupt routine */
/* void ISRH_Handler(void){

  attachInterrupt(digitalPinToInterrupt(ENC2APin), handleInterrupt(), CHANGE);
  //HAL_GPIO_EXTI_IRQHandler(gpio13);
  BaseType_t checkIfYieldRequired;
  checkIfYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
  portYIELD_FROM_ISR(checkIfYieldRequired);

} */

void handleInterrupt(){
  vTaskResume(myIntTaskHandle);
}

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR ISRH_Handler() {
  portENTER_CRITICAL_ISR(&mux);

  // Your ISR code here
  attachInterrupt(digitalPinToInterrupt(ENC2APin), handleInterrupt, CHANGE);

  BaseType_t checkIfYieldRequired;
  checkIfYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
  portYIELD_FROM_ISR(checkIfYieldRequired);

  portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR ISRH_HandlerGPT() {
    portENTER_CRITICAL_ISR(&mux);

    // Your ISR code here
    
    EncoderUpdate();

    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
    portYIELD_FROM_ISR(checkIfYieldRequired);

    portEXIT_CRITICAL_ISR(&mux);
}

void PollEnc(void* parameter){
for(;;){
  Serial.println(motor1.encoderPosition);
  Serial.println(motor2.encoderPosition);
  vTaskDelay(1000);

}
}

//Encoder update loop 
void enc_loop2(void* parameter){

  attachInterrupt(ENCAPin, bs, CHANGE);
  attachInterrupt(ENCBPin, bs, CHANGE);

  for(;;){
    vTaskDelay(1000);
  }
}

void setpostask(void* parameter){
  
  //motor1.goalpos=100;
  //motor1.setpos(100,60);
  motor1.setgoalpos(100);
  motor1.setposPID();

}

void but_task(void* parameter){
  for(;;){
    while(interruptFlag==true){
      vTaskDelay(10);
    }

    ( digitalRead(button) == HIGH) ?  motor1.move(100): motor1.move(200);

    vTaskDelay(10);
  }


}


//___________________Setup and Loop Functions___________________

void setup() {
  Serial.begin(115200);
  Serial.print("Starting");
  pinMode(button, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC2APin), ISRH_HandlerGPT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2BPin), ISRH_HandlerGPT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCAPin), ISRH_HandlerGPT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCBPin), ISRH_HandlerGPT, CHANGE);
/*   attachInterrupt(ENCAPin, bs, CHANGE);
  attachInterrupt(ENCBPin, bs, CHANGE);

  attachInterrupt(ENC2APin, bs, CHANGE); 
  attachInterrupt(ENC2BPin, bs, CHANGE); */

  //motor1.move(100);
  //boolean E = motor1.getenc();
  //Serial.print(E);
  //xTaskCreate (Baitandswitch, "A",  4096, NULL, 1, NULL);
  //xTaskCreate (enc_loop, "ENC",  4096, NULL, 2, NULL);
  xTaskCreate (setpostask, "setpos",  4096, NULL, 1, NULL);
  xTaskCreate (PollEnc, "Hope",  4096, NULL, 1, NULL);
  //xTaskCreate (but_task, "move",  4096, NULL, 1, NULL);
  xTaskCreate (myInterruptTask, "Interrupt",  4096, NULL, 1, &myIntTaskHandle);

  //vTaskStartScheduler();
  
  //motor1.move(100);
  //motor2.move(200);

}
 
void loop() {
  delay(1000);
  
}  