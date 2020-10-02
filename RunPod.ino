// Include the necessary libraries 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SavLayFilter.h>
#include <Wire.h>

// Instantiate the accelerometer class
Adafruit_MPU6050 mpu;


double z_gyro;
double last_check = 0;
double derivative = 0;


//SavLayFilter smallFilter (&value, 0, 5);             //Cubic smoothing with windowsize of 5
SavLayFilter largeFilter (&z_gyro, 0, 25);            //Cubic smoothing with windowsize of 25
SavLayFilter derivativeFilter (&derivative, 0, 25); 


void setup(void) {

    Serial.begin(115200);

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

    double z_array[5];
    double d_array[10];

    for(auto& z:z_array){
        /* Get new sensor events with the readings */
        sensors_event_t accel, gyro, temp;
        mpu.getEvent(&accel, &gyro, &temp);
        z_gyro = gyro.gyro.z;

        
        // Print the raw data
        // Serial.print("Raw_Data:");
        // Serial.print(z_gyro);
        
        // Serial.print(",");
        
        // Compute the Savitzky-Golay Filtering Algorithm to smooth the z angular acceleration
        z_gyro = largeFilter.Compute();
        z_gyro = largeFilter.Compute();
        // Serial.print("Smoothed_Data_1:");
        // Serial.print(z_gyro);

        // Add the smoothed data to the array
        z = z_gyro;
    
        
        
    }

    // Sum the z data and make an average
    double sum = 0;
    for(auto& n:z_array){
        sum+=n;
    }

    double average = sum/5;
    Serial.print("Average_5:");
    Serial.print(average);
    Serial.print(",");

    derivative = average - last_check;
    derivative = derivativeFilter.Compute();
    derivative = derivativeFilter.Compute();
    derivative = derivativeFilter.Compute();
    Serial.print("Derivative:");
    Serial.println(derivative * 10);
    
    last_check = average;
    
}
