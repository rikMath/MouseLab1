/* Mouse BLE */

#include <BleMouse.h>
BleMouse bleMouse;

/* Adafruit MPU6050 */

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu; //0x68

#define mpu_scl 22
#define mpu_sda 21
TwoWire I2Cone = TwoWire(1);

#include "heltec.h"

/* Mouse Hardware*/

#define LBUT 36
#define RBUT 37

/* Tratamento MPU p/ Mouse */

#define ELAPSED_TIME 0.01 // Usar no main a declaração: elapsedTime = (currentTime - previousTime) / 1000;

#define GYRO_PERCENTAGE 0.96
#define ACC_PERCENTAGE (1-GYRO_PERCENTAGE)

#define MOUSE_SENSITIVITY 1000

typedef struct 
{
    float previous_gyro_x;
    float previous_gyro_y;
} MPU_INPUT_CONVERTER;

MPU_INPUT_CONVERTER mpu_converter; 

float get_mouse_variation(MPU_INPUT_CONVERTER*, float, float, float);

void startMPU(){

  /* MPU init */
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

  bleMouse.begin();

  mpu_converter.previous_gyro_x = -0.02; // Inserir erro
  mpu_converter.previous_gyro_y = 0.04; // Inserir erro
  
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

  /* Print out the variations values */
  float mouse_var_x = get_mouse_variation_x(&mpu_converter, g.gyro.x, a.acceleration.x, ELAPSED_TIME);
  float mouse_var_y = get_mouse_variation_y(&mpu_converter, g.gyro.y, a.acceleration.y, ELAPSED_TIME);

  Serial.print("g gyro x: ");
  Serial.println(g.gyro.x);
  Serial.print("Variação Mouse X: ");
  Serial.println(mouse_var_x);
  Serial.print("Variação Mouse Y: ");
  Serial.println(mouse_var_y);
  Serial.println("");


  

  bleMouse.move(mouse_var_x,mouse_var_y);
  
  delay(ELAPSED_TIME*1000);
}

// Implementação funções auxiliares

float get_mouse_variation_x(MPU_INPUT_CONVERTER* mpu_converter, float gyro_x_rad_sec, float acc_x_rad_sec, float elapsed_time) {

  float corrected_gyro_x = gyro_x_rad_sec - mpu_converter->previous_gyro_x;

  Serial.print("gyro_x_rad_sec: ");
  Serial.println(gyro_x_rad_sec);
  Serial.print("corrected_gyro_x: ");
  Serial.println(corrected_gyro_x);

  float variation = (corrected_gyro_x * GYRO_PERCENTAGE * elapsed_time) + (acc_x_rad_sec * ACC_PERCENTAGE * elapsed_time);

  return variation * MOUSE_SENSITIVITY;
}

float get_mouse_variation_y(MPU_INPUT_CONVERTER* mpu_converter, float gyro_y_rad_sec, float acc_y_rad_sec, float elapsed_time) {

  float corrected_gyro_y =  gyro_y_rad_sec - mpu_converter->previous_gyro_y;

  

  float variation = (corrected_gyro_y * GYRO_PERCENTAGE * elapsed_time) + (acc_y_rad_sec * ACC_PERCENTAGE * elapsed_time);

  return variation * MOUSE_SENSITIVITY;
}
