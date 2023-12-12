#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


//int digitalPin = 32; // Change this to the pin you're using
int IRPin = 39; // Change this to the pin you're using
int button=34;
int value = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(0x29)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}




void loop() {
  // put your main code here, to run repeatedly:
  //for TOF sensor https://electronoobs.com/eng_arduino_tut73.php
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);



  value = analogRead(IRPin);
  Serial.print("IR object detection: ");
  Serial.println(value);


  delay(1000); // Read the pin value every second
}

