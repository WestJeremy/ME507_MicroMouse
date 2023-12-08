#include <Arduino.h>
#include "Motor.h"
#include "States.h"
#include "Micromouse.h"
#include "MPU9250.h"


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

//Sensors
#define IR_L_Pin 39
#define IR_R_Pin 36

int tof_address = 0x29;

// Global variable shared between ISR and task
volatile bool interruptFlag = false;

int cur_state=0;
int micromouse_states=2;

boolean x=1;
uint8_t pos=0;
uint8_t curpos=0;

//Object initilization
Motor motor1(PWM1_pin, AIN1_pin , AIN2_pin,ENCAPin, ENCBPin ); 
Motor motor2(PWM2_pin, BIN1_pin , BIN2_pin,ENC2APin, ENC2BPin );

IR IR_L(IR_L_Pin);
IR IR_R(IR_R_Pin);

Time_Of_Flight TOF(tof_address);

MPU9250 IMU(Wire, 0x68);
int status;

Micromouse Mouse(&motor1, &motor2 , &IR_L , &IR_R , &TOF , &IMU);

State state(micromouse_states);


//Task Handles
TaskHandle_t myIntTaskHandle = NULL;
TaskHandle_t buttonIntTaskHandle = NULL;

// ___________________Functions___________________

//Update Both Encoders
void EncoderUpdate(){
  motor1.updateEncoder();
  motor2.updateEncoder();
}

//Task loop For Encoder Interrupt
void myInterruptTask(void* parameter){
  //Task resumes on interrupt then suspends self
  for(;;){
    vTaskSuspend(NULL); //suspend self
    //Code to run on intterupt goes below
    EncoderUpdate();
   
  }
}

//Task loop For Button Interrupt
void buttonInterruptTask(void* parameter){
  //Task resumes on interrupt then suspends self
  for(;;){
    vTaskSuspend(NULL); //suspend self
    //Code to run on interupt goes below
    state.update();
    cur_state=state.get();
    //Serial.print(cur_state);
  }
}

void handleInterrupt(){
  vTaskResume(myIntTaskHandle);
}

void handleButtonInterrupt(){
  vTaskResume(buttonIntTaskHandle);
}

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR ISRH_Handler_button() {
    portENTER_CRITICAL_ISR(&mux);

    // Your ISR code here
    //handleButtonInterrupt();
    

    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(buttonIntTaskHandle);
    portYIELD_FROM_ISR(checkIfYieldRequired);

    portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR ISRH_Handler_Encoder() {
    portENTER_CRITICAL_ISR(&mux);

    // Your ISR code here
    //EncoderUpdate();

    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
    portYIELD_FROM_ISR(checkIfYieldRequired);

    portEXIT_CRITICAL_ISR(&mux);
}

void PollSensors(void* parameter){
for(;;){
  if (state.get()==0 ){
  Serial.print("Motor 1: ");
  Serial.print(motor1.encoderPosition);
  Serial.print("  ");

  Serial.print("Motor 2: ");
  Serial.print(motor2.encoderPosition);
  Serial.println("  ");

  Serial.print("State: ");
  Serial.println(cur_state);
/* 
  Serial.print("IR Left: ");
  Serial.print(IR_L.get());
  Serial.print("  ");
  Serial.print("IR Right: ");
  Serial.println(IR_R.get());
  
  Serial.print("TOF: ");
  Serial.println(TOF.get());

  Serial.print("IMU Z: ");*/
  IMU.readSensor();
/*   Serial.print("X:  ");
  Serial.print("  ");
  Serial.print(IMU.getMagX_uT(), 6);//roll 
  Serial.print("  ");
  Serial.print("Y:  ");
  Serial.print("  ");
  Serial.print(IMU.getMagY_uT(), 6);//yaw
  Serial.print("  ");
  Serial.print("Z:  ");
  Serial.println(IMU.getMagZ_uT(), 6);//pitch */


  Serial.print("X:  ");
  Serial.print("  ");
  Serial.print(IMU.getGyroX_rads(), 6);
  Serial.print("  ");
  Serial.print("Y:  ");
  Serial.print("  ");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("  ");
  Serial.print("Z:  ");
  Serial.print(IMU.getGyroZ_rads(), 6);}



 

  vTaskDelay(100);

}
}


void Gotask(void* parameter){
  int Cells=1;
  int Moveops=2;
  Mouse.GoalAngle=0;
  State MoveState(Moveops);

  MoveState.update();

  
  for(;;){
  int cur_state=state.get();

  if(cur_state >= 1){
    vTaskDelay(1000);
    if (MoveState.get()==1){
      Mouse.MoveStable(Cells);
      MoveState.set(0);
      //Serial.print("Started PID");

    }

    if (MoveState.get()==2){
      Mouse.RotStable(90);
      MoveState.set(0);
      //Serial.print("Started PID");

    }

    vTaskDelay(10);
    
    

  }else{
    Serial.print("State Zero: Stand by -o- ");
    motor1.stop();
    motor2.stop();
    vTaskDelay(1000);
    
  }
  }
  vTaskDelay(10);
}





void setpostask(void* parameter){
  for(;;){
  int cur_state=state.get();
  if(cur_state == 1){
  motor1.setgoalpos(10000);
  motor2.setgoalpos(10000);
  motor1.stepsetposPID();
  motor2.stepsetposPID();
  }

  else{
    //Serial.print("State Zero: Stand by -o- ");
    motor1.stop();
    motor2.stop();
    vTaskDelay(100);
    
  }
  }
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

  //Start Communication with TOF
  TOF.start();

  // start communication with IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }

  Mouse.calIMU();
  Mouse.zeroenc();

  



  //Encoder ISRs
  attachInterrupt(digitalPinToInterrupt(ENC2APin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2BPin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCAPin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCBPin), ISRH_Handler_Encoder, CHANGE);
  Serial.println("1");
  //Button ISR for state control
  attachInterrupt(digitalPinToInterrupt(button), ISRH_Handler_button,RISING);
  Serial.println("2");

  //Tasks
  //xTaskCreate (setpostask, "setpos",  4096, NULL, 3, NULL);
  xTaskCreate (Gotask, "setpos",  4096, NULL, 3, NULL);


  xTaskCreate (PollSensors, "Poll_Sensors",  2000, NULL, 1, NULL);

  Serial.println("3");
  xTaskCreate (myInterruptTask, "Encoder ISR",  4096, NULL, 1, &myIntTaskHandle);
  xTaskCreate (buttonInterruptTask, "Button Interrupt",  4096, NULL, 2, &buttonIntTaskHandle);

  Serial.println("4");
}
 
void loop() {
  delay(1000);
  
}  