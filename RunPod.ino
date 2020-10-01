// Include the necessary libraries 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Instantiate the accelerometer class
Adafruit_MPU6050 mpu;

void setup(void) {

    Serial.begin(115200);
    while (!Serial) {
        delay(10); // will pause Zero, Leonardo, etc until serial console opens
    }


    // Try to initialize, if it fails it will continue to try and throw an error message
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
        delay(10);
        }
    }

    // Set all the default values for the accelerometer parameters 
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

void loop() {

  float smoothed_data[] = {0,0,0,0,0,0,0,0,0,0};
  for (auto& smoothed:smoothed_data){
    float x_values[] = {0,0,0,0,0,0,0,0,0,0};
    
    for (auto& value:x_values){
      /* Get new sensor events with the readings */
      sensors_event_t accel, gyro, temp;
      mpu.getEvent(&accel, &gyro, &temp);
      value = accel.acceleration.x;
  
    }
  
    float average = 0;
    
    for (auto& reading:x_values){
      average+=reading;
    }
  
    smoothed = average/10;
    
    

  } 

  float average = 0;
   for (auto& smoothed:smoothed_data){
      average+=smoothed;
    }

  Serial.print("Smoothed_data:");
  Serial.println(average/10);
}
