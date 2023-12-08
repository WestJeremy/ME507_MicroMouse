#include "Micromouse.h"



Micromouse::Micromouse(Motor* motor1, Motor* motor2, IR* IR_L,IR* IR_R,Time_Of_Flight* TOF, MPU9250* IMU) : motor1_(motor1), motor2_(motor2), IR_L_(IR_L), IR_R_(IR_R),TOF_(TOF),IMU_(IMU){
    
}



void Micromouse::calIMU() {
  void setMagCalX(float getMagBiasX_uT(),float getMagScaleFactorX());
  void setMagCalX(float getMagBiasX_uT(),float getMagScaleFactorX());
  void setMagCalX(float getMagBiasX_uT(),float getMagScaleFactorX());
  void setMagCalY(float getMagBiasY_uT(),float getMagScaleFactorY());
  void setMagCalY(float getMagBiasY_uT(),float getMagScaleFactorY());
  void setMagCalY(float getMagBiasY_uT(),float getMagScaleFactorY());
  void setMagCalZ(float getMagBiasZ_uT(),float getMagScaleFactorZ());
  void setMagCalZ(float getMagBiasZ_uT(),float getMagScaleFactorZ());
  void setMagCalZ(float getMagBiasZ_uT(),float getMagScaleFactorZ());


}


void Micromouse::Move(int Cells) {

  int counts=Cells*7200;
  Serial.print(counts);
  motor1_ -> zeroEncPos();
  motor2_ -> zeroEncPos();
  //float Neterror= IMU_->getMagY_uT() ;
  motor1_ -> setgoalpos(counts);
  motor2_ -> setgoalpos(counts);

  while(motor2_ -> encoderPosition < counts){
    motor1_ -> stepsetposPID();
    motor2_ -> stepsetposPID();
    vTaskDelay(10);
  }
  

}

void Micromouse::MoveStable(int Cells) {
  
  int counts=Cells*7200;
  Serial.print(counts);
  motor1_ -> zeroEncPos();
  motor2_ -> zeroEncPos();
  //float Neterror= IMU_->getMagY_uT() ;
  motor1_ -> setgoalpos(counts);
  motor2_ -> setgoalpos(counts);

  float U = StepAngularPID();
  Serial.println("Entering loop: ");

  float ErrorAllowable=counts*.05;
  float AngleErrorAllowable=.4;

  while(abs(motor1_ -> encoderPosition - motor1_ -> goalpos) > ErrorAllowable && abs(motor2_ -> encoderPosition - motor2_ -> goalpos) > ErrorAllowable ){
    //while(1){
    IMU_->readSensor();
    AngError=AngError+IMU_->getGyroZ_rads();
    Serial.print("AngularError: ");
    Serial.print(AngError);
    Serial.print("  ");


    U = StepAngularPID();
    motor1_ -> stepsetposPIDU(U);
    motor2_ -> stepsetposPIDU(-U);

/*     Serial.print("U: ");
    Serial.print(U);
    Serial.print("  "); */
    vTaskDelay(10);
  }
  stop();

}

void Micromouse::RotStable(float angle) {
  GoalAngle=angle*42/90;
  AngError=GoalAngle;
  Serial.print("AngularError: ");
  Serial.print(AngError);
  Serial.print("  ");
  
  motor1_ -> zeroEncPos();
  motor2_ -> zeroEncPos();

  motor1_ -> setgoalpos(0);
  motor2_ -> setgoalpos(0);

  float U = StepAngularPID();
  Serial.println("Entering loop: ");

  float AngleErrorAllowable=.4;
// abs(GoalAngle) > abs(AngError) 
  while(abs(AngError)>.4){
    IMU_->readSensor();
    AngError=AngError+IMU_->getGyroZ_rads();

    Serial.print("AngularError: ");
    Serial.print(AngError);
    Serial.print("  ");
    zeroenc();

    U = StepAngularPID();
    motor1_ -> stepsetposPIDU(U);
    motor2_ -> stepsetposPIDU(-U);

    vTaskDelay(10);
  }
  stop();

  

}




float Micromouse::StepAngularPID() {
    IMU_ -> readSensor();
    int t_step=10;
    float U_error;
    float prevError=AngError;

    AngError= AngError;
    
    I_error=0;
    float dt_error=0;
    float d_error=0;



    float kp = 5;
    float ki=0;
    float kd=0;

    float t_in;

    

        I_error=I_error+AngError;
        d_error=AngError-prevError/t_step;

        float p_in=AngError*kp;
        float i_in=I_error*ki;
        float d_in=d_error*kd;

        U_error=p_in+i_in+d_in;
        U_error=(U_error > 1000) ? 1000 : U_error; //if the controler input is oversaturated write it to 100%


        vTaskDelay(t_step);
        return U_error;
    
}

void Micromouse:: zeroenc(){
  motor1_-> zeroEncPos();
  motor2_-> zeroEncPos();
}

void Micromouse:: stop(){
  motor1_-> stop();
  motor2_-> stop();
}

