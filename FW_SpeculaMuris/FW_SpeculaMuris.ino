//Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu; //0x68

#define mpu_scl 22
#define mpu_sda 21
TwoWire I2Cone = TwoWire(1);

#include "heltec.h"

//Mouse

#include <Mouse.h>

#define LBUT 36
#define RBUT 37

void startMPU(){
  I2Cone.begin(mpu_sda, mpu_scl, 100000); 
  mpu.begin(0x68, &I2Cone);  
  
  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU cicle finish!");
}

void setup(void) {
  Serial.begin(115200);

  startMPU();

  pinMode(LBUT, INPUT);
  //digitalWrite(LBUT, HIGH);
  pinMode(RBUT, INPUT);
  //digitalWrite(RBUT, HIGH);
  
  Serial.println("Setup finish!");
  Serial.println("MPU6050 OLED demo init");
  
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}
