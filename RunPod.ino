// Include the necessary libraries 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LinkedList.h>
#include <Gaussian.h>
#include <GaussianAverage.h>
#include <Wire.h>

#include "DataAnalysis.h"
#include "DataSet.h"

// Macros during Development
#define log(x) Serial.print(x)
#define log_ln(x) Serial.println(x)

// consts that will be used as a thrashold during data analysis 
const double x_acceleration_thrashold = 2; //* If the x_acceleration is withing +-2 it will not be considered a step

// Instantiate the accelerometer class
Adafruit_MPU6050 mpu;

// Instantiate all the objects of the Gaussian wrapper to smooth the raw accelerometer data 
GaussianAverage x_acceleration_average = GaussianAverage(100); //* Average of 100 datapoints to smooth the x accel data
GaussianAverage y_acceleration_average = GaussianAverage(100); //* Average of 100 datapoints to smooth the y accel data


// Variables used to preserve the gyro data from one iteration to the next
double x_acceleration = 0; //* Raw data from the accelerometer on the x axis
double y_acceleration = 0; //* Raw data from the accelerometer on the y axis
double last_check = 0;     //* Value to store the acceleration from one iteration to the next (for the derivative calculations)
double derivative = 0;     //* Value that will store the derivative of the x acceleration in respect to time

// Data sets that will be used to store raw values of the accel module so we can have acces to the actual max values
Data_Set<double,100> raw_x_dataset; //* Will store the actual raw values for the x accelerometer
Data_Set<double,100> raw_y_dataset; //* Will store the actual raw values for the y accelerometer


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

    
    /* Get new sensor events with the readings */
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    x_acceleration = accel.acceleration.x;
    y_acceleration = accel.acceleration.y;

    // Add the raw accelerometer data to the dataset for future analysis
    raw_x_dataset.push_back(x_acceleration);
    raw_y_dataset.push_back(y_acceleration);
    
    // Add the acceleration raw data to the gaussian's datasets to make a new mean 
    x_acceleration_average+=x_acceleration; //* Add to the x_accel dataset 
    y_acceleration_average+=y_acceleration; //* Add to the y_accel dataset 
        
    // Process the new gaussian average with the new datapoints added to their respective datasets 
    x_acceleration_average.process();
    y_acceleration_average.process();

    // Calculate the derivative for the x_acceleration and update the last_check variable for next iteration 
    derivative = x_acceleration_average.mean - last_check;
    last_check = x_acceleration_average.mean;

    /*  If the derivative is zero it means it is in a max/min point, we need to check if the x_acceleration
        is outside the thrashold. If it is outside, we consider it a step, otherwise we consider it just
        an outlier.
    */
    if (derivative == 0 && !Data_Analysis::inside_thrashold(x_acceleration_average.mean, x_acceleration_thrashold)){
        
        // calculate the angle based on the y_accel
        // Send \theta and a step notification to the app

    }
    
    // Print all the values for plotting and debuging
    log("x_accel_raw:");
    log(x_acceleration);
    log(",");

    log("y_accel_raw:");
    log(y_acceleration);
    log(",");

    log("x_average:");
    log(x_acceleration_average.mean);
    log(",");

    log("y_average:");
    log(y_acceleration_average.mean);
    log(",");

    log("derivative:");
    log_ln(derivative);
        
    
}
