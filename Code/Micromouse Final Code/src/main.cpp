/**
 * @file main.cpp
 * @brief Micromouse main application.
 *  @author Jeremy West
 *  @date   2023-Nov-3
 */

#include <Arduino.h>
#include "Motor.h"
#include "States.h"
#include "Micromouse.h"
#include "MPU9250.h"
#include "Task_Floodfill.h"
#include "WebsocketInterface.h"


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





State state(micromouse_states);

Micromouse Mouse(&motor1, &motor2 , &IR_L , &IR_R , &TOF , &IMU, &state);

Solver solve(&Mouse);


// SSID and password of Wifi connection:
const char* ssid = "ID";
const char* password = "PASS";

// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
String webpage = "<!DOCTYPE html><html><head><title>Page Title</title></head><body style='background-color: #EEEEEE;'><span style='color: #003366;'><h1>Micromouse Websockets Interface</h1> <p>Motor 1 position: <span id='rand1'>-</span></p><p>Motor 2 position: <span id='rand2'>-</span></p><p>State: <span id='state'>-</span></p><p><button type='button' id='BTN_SEND_BACK'>Update State</button></p></span></body><script> var Socket; document.getElementById('BTN_SEND_BACK').addEventListener('click', button_send_back); function init() { Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function(event) { processCommand(event); }; } function button_send_back() { var msg = {inc: 1,};Socket.send(JSON.stringify(msg)); } function processCommand(event) {var obj = JSON.parse(event.data);document.getElementById('rand1').innerHTML = obj.rand1;document.getElementById('rand2').innerHTML = obj.rand2; document.getElementById('state').innerHTML = obj.state; console.log(obj.rand1);console.log(obj.rand2); console.log(obj.state); } window.onload = function(event) { init(); }</script></html>";
WebsocketConnection BeepBoopStream(ssid, password, webpage,&Mouse);

//Task Handles
TaskHandle_t myIntTaskHandle = NULL;
TaskHandle_t buttonIntTaskHandle = NULL;

// ___________________Functions___________________

/**
 * @brief Function to update both encoders.
 */
void EncoderUpdate(){

  Mouse.updateEncoder();
}

/**
 * @brief Task loop for encoder interrupt.
 * 
 * @param parameter Task parameter (not used).
 */
void myInterruptTask(void* parameter){
  //Task resumes on interrupt then suspends self
  for(;;){
    vTaskSuspend(NULL); //suspend self
    //Code to run on intterupt goes below
    EncoderUpdate();
   
  }
}

/**
 * @brief Task loop for button interrupt.
 * 
 * @param parameter Task parameter (not used).
 */
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

/**
 * @brief resume encoder task.
 */
void handleInterrupt(){
  vTaskResume(myIntTaskHandle);
}

/**
 * @brief resume button task.
 */
void handleButtonInterrupt(){
  vTaskResume(buttonIntTaskHandle);
}

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


/**
 * @brief ISR handler for button interrupt.
 */
void IRAM_ATTR ISRH_Handler_button() {
    portENTER_CRITICAL_ISR(&mux);

    // Your ISR code here
    //handleButtonInterrupt();
    

    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(buttonIntTaskHandle);
    portYIELD_FROM_ISR(checkIfYieldRequired);

    portEXIT_CRITICAL_ISR(&mux);
}

/**
 * @brief ISR handler for encoder interrupt.
 */
void IRAM_ATTR ISRH_Handler_Encoder() {
    portENTER_CRITICAL_ISR(&mux);

    // Your ISR code here
    //EncoderUpdate();

    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
    portYIELD_FROM_ISR(checkIfYieldRequired);

    portEXIT_CRITICAL_ISR(&mux);
}

/**
 * @brief Poll all sensors, and states and print to serial.
 */
void PollSensors(){

  Serial.print("Motor 1: ");
  Serial.print(motor1.encoderPosition);
  Serial.print("  ");

  Serial.print("Motor 2: ");
  Serial.print(motor2.encoderPosition);
  Serial.println("  ");

  Serial.print("State: ");
  Serial.println(cur_state);

  Serial.print("IR Left: ");
  Serial.print(IR_L.get());
  Serial.print("  ");
  Serial.print("IR Right: ");
  Serial.println(IR_R.get());
  
  Serial.print("TOF: ");
  Serial.println(TOF.get());

  Serial.print("IMU Z: ");
  IMU.readSensor();
  Serial.print("X:  ");
  Serial.print("  ");
  Serial.print(IMU.getMagX_uT(), 6);//roll 
  Serial.print("  ");
  Serial.print("Y:  ");
  Serial.print("  ");
  Serial.print(IMU.getMagY_uT(), 6);//yaw
  Serial.print("  ");
  Serial.print("Z:  ");
  Serial.println(IMU.getMagZ_uT(), 6);//pitch


  Serial.print("X:  ");
  Serial.print("  ");
  Serial.print(IMU.getGyroX_rads(), 6);
  Serial.print("  ");
  Serial.print("Y:  ");
  Serial.print("  ");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("  ");
  Serial.print("Z:  ");
  Serial.print(IMU.getGyroZ_rads(), 6);

}

/**
 * @brief Main state system dispatcher for the microcmouse
 */
void Gotask(void* parameter){
  int Cells=1;
  int Moveops=2;
  Mouse.GoalAngle=0;
  State MoveState(Moveops);

  MoveState.update();

  
  for(;;){
  int cur_state=state.get();
  Serial.println(cur_state);


  if(cur_state >= 1){
    vTaskDelay(100);
    
    if (cur_state ==1){
      solve.task_floodfill();
      state.set(0);
    }

    if (cur_state ==2){
      PollSensors();
      
      //Serial.print("Started PID");

    }

  }else{
    Serial.print("  State Zero: Stand by -o-  ");
    motor1.stop();
    motor2.stop();
    vTaskDelay(500);
    
  }
  }
  vTaskDelay(10);
}




/**
 * @brief test software to set arbitrary goal position
 */
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
    motor1.stop();
    motor2.stop();
    vTaskDelay(100);
    
  }
  }
}

/**
 * @brief task wrapper for webserver init
 */
void webservertask(void* parameter){
  BeepBoopStream.task();
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

  Mouse.calIMU(); //calibrate IMU
  Mouse.zeroenc(); //Zero encoders

  //Start webserver
  BeepBoopStream.Start();

  //Encoder ISRs
  attachInterrupt(digitalPinToInterrupt(ENC2APin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2BPin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCAPin), ISRH_Handler_Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCBPin), ISRH_Handler_Encoder, CHANGE);

  //Button ISR for state control
  attachInterrupt(digitalPinToInterrupt(button), ISRH_Handler_button,RISING);
 

  //Tasks
  xTaskCreate (Gotask, "Go Task",  4096, NULL, 3, NULL);
  xTaskCreate (myInterruptTask, "Encoder ISR",  4096, NULL, 1, &myIntTaskHandle);
  xTaskCreate (buttonInterruptTask, "Button Interrupt",  4096, NULL, 2, &buttonIntTaskHandle);
  xTaskCreate (webservertask, "Webserver Task",  4096, NULL, 2, NULL);
}
 
void loop() {
  delay(1000);
  
}  