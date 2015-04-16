#include <Wire.h>
#include <I2Cdev.h>
#include <DualVNH5019MotorShield.h>
#include "helper_3dmath.h"
#include "MPU9150.h"

MPU9150 mpu;
DualVNH5019MotorShield md;

int16_t ax, ay, az;    //各軸方向の加速度
int16_t gx, gy, gz;    //各軸周りの角速度
int16_t mx, my, mz;  //各軸方向の地磁気
 
void stopIfFault(){
    if (md.getM1Fault()){
        Serial.println("M1 fault");
        while(1);
    }
    if (md.getM2Fault()){
        Serial.println("M2 fault");
        while(1);
    }
}

void setup() {
    Wire.begin();
    Serial.begin(9600);
    Serial.println("VHN5019 Motor Shield + MPU9150 sensor");
    //VHN5019の初期化
    md.init();
    //MPU9150の初期化
    mpu.initialize();
    Serial.println("Initialized");
}

void loop() {
    //各センサの値を取得
    mpu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.println(mz);

    //Serial.print("gyroscope X-axis value = ");
    //Serial.println(gx); 
    //MPU9150で取得したｘ軸周りの回転と逆向きにモータM1を回転させる
    float gxSpeed = 250.0 * (float)gx / 32768.0;
    Serial.print("angular velocity X-axis = ");
    Serial.println(gxSpeed);
    float gySpeed = 250.0 * (float)gy / 32768.0;
    Serial.print("angular velocity Y-axis = ");
    Serial.println(gySpeed);
    float gzSpeed = 250.0 * (float)gz / 32768.0;
    Serial.print("angular velocity Z-axis = ");
    Serial.println(gzSpeed);
    int m1Speed;
    if(gy > 400){
        m1Speed = 200;
    }else if(gy < -400){
        m1Speed = -200;
    }else{
        m1Speed = gy / 10;
    }
    Serial.println(m1Speed);
    md.setM1Speed(m1Speed);
    stopIfFault();
    Serial.println(md.getM1CurrentMilliamps());
    delay(300);
}
