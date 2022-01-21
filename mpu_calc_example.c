#include <stdio.h>

#define ELAPSED_TIME 0.01 // Usar no main a declaração: elapsedTime = (currentTime - previousTime) / 1000;

#define GYRO_PERCENTAGE 0.96
#define ACC_PERCENTAGE (1-GYRO_PERCENTAGE)

#define MOUSE_SENSITIVITY 10

typedef struct 
{
    float previous_gyro_x;
    float previous_gyro_y;
} MPU_INPUT_CONVERTER;

float get_mouse_variation_x(MPU_INPUT_CONVERTER*, float, float, float);
float get_mouse_variation_y(MPU_INPUT_CONVERTER*, float, float, float);

int main(int argc, char const *argv[])
{
  MPU_INPUT_CONVERTER mpu_converter;

  mpu_converter.previous_gyro_x = 0.0; // Inserir erro
  mpu_converter.previous_gyro_y = 0.0; // Inserir erro

  printf("Valor x: %f\n", get_mouse_variation_x(&mpu_converter, 0.10, -0.02, ELAPSED_TIME));
  printf("Valor y: %f\n", get_mouse_variation_y(&mpu_converter, -0.12, -0.5, ELAPSED_TIME));

  return 0;
}

float get_mouse_variation_x(MPU_INPUT_CONVERTER* mpu_converter, float gyro_x_rad_sec, float acc_x_rad_sec, float elapsed_time) {

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