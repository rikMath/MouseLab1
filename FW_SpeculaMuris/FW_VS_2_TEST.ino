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

// Tratamento MPU p/ Mouse
#define ELAPSED_TIME 0.01 // Usar no main a declaração: elapsedTime = (currentTime - previousTime) / 1000;

#define GYRO_PERCENTAGE 0.96
#define ACC_PERCENTAGE (1-GYRO_PERCENTAGE)

#define MOUSE_SENSITIVITY 10

typedef struct 
{
    float previous_gyro_x;
    float previous_gyro_y;
} MPU_INPUT_CONVERTER;

float get_mouse_variation(MPU_INPUT_CONVERTER*, float, float, float);

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
  
  // XXX: A seguinte declaração funciona aqui ou precisa ser fora do escopo? No código em c puro usei no main.
  MPU_INPUT_CONVERTER mpu_converter; // Declaração necessária no setup mas é utilizado no loop, funciona assim?

  mpu_converter.previous_gyro_x = 0.0; // Inserir erro
  mpu_converter.previous_gyro_y = 0.0; // Inserir erro

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


  float mouse_var_x = get_mouse_variation_x(&mpu_converter, g.gyro.x, a.acceleration.x, ELAPSED_TIME);
  float mouse_var_y = get_mouse_variation_y(&mpu_converter, g.gyro.y, a.acceleration.y, ELAPSED_TIME);

  Serial.print("Variação Mouse X: ");
  Serial.println(mouse_var_x);
  Serial.print("Variação Mouse Y: ");
  Serial.println(mouse_var_y);

  delay(500);
}

// Implementação funções auxiliares

float get_mouse_variation(MPU_INPUT_CONVERTER* mpu_converter, float gyro_x_rad_sec, float acc_x_rad_sec, float elapsed_time) {

  float new_gyro_x = mpu_converter->previous_gyro_x + gyro_x_rad_sec * elapsed_time;

  mpu_converter->previous_gyro_x = new_gyro_x;

  float variation = (new_gyro_x * GYRO_PERCENTAGE) + (acc_x_rad_sec * ACC_PERCENTAGE);

  return variation * MOUSE_SENSITIVITY;
}

float get_mouse_variation_y(MPU_INPUT_CONVERTER* mpu_converter, float gyro_y_rad_sec, float acc_y_rad_sec, float elapsed_time) {

  float new_gyro_y = mpu_converter->previous_gyro_y + gyro_y_rad_sec * elapsed_time;

  mpu_converter->previous_gyro_y = new_gyro_y;

  float variation = (new_gyro_y * GYRO_PERCENTAGE) + (acc_y_rad_sec * ACC_PERCENTAGE);

  return variation * MOUSE_SENSITIVITY;
}