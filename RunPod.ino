// Include the necessary libraries 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LinkedList.h>
#include <Gaussian.h>
#include <GaussianAverage.h>
#include <Wire.h>
#include <cmath>

/// WiFi webserver connection /// 
#include <WiFi.h>
#include <HTTPClient.h>
// 
const char* ssid = "VIVO-6464";
const char* password = "C6624A6464";


#include "DataAnalysis.h"
#include "DataSet.h"

// Macros during Development
#define log(x) Serial.print(x)
#define log_ln(x) Serial.println(x)

// consts that will be used as a thrashold during data analysis 
const double x_acceleration_thrashold = 1; //* If the x_acceleration is withing +-2 it will not be considered a step
const double z_velocity_thrashold = 0.3;

// Instantiate the accelerometer class
Adafruit_MPU6050 mpu;

// Instantiate all the objects of the Gaussian wrapper to smooth the raw accelerometer data 
GaussianAverage x_acceleration_average = GaussianAverage(100); //* Average of 100 datapoints to smooth the x accel data
GaussianAverage y_acceleration_average = GaussianAverage(100); //* Average of 100 datapoints to smooth the y accel data
GaussianAverage z_velocity_average     = GaussianAverage(100);

// Variables used to preserve the gyro data from one iteration to the next
double x_acceleration = 0; //* Raw data from the accelerometer on the x axis
double y_acceleration = 0; //* Raw data from the accelerometer on the y axis
double last_check = 0;     //* Value to store the acceleration from one iteration to the next (for the derivative calculations)
double derivative = 0;     //* Value that will store the derivative of the x acceleration in respect to time
double z_velocity = 0;     //* Raw data from the gyroscope


// Data sets that will be used to store raw values of the accel module so we can have acces to the actual max values


void compute_acceleration(){

    /* Get new sensor events with the readings */
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    x_acceleration = accel.acceleration.x;
    y_acceleration = accel.acceleration.y;
    z_velocity = gyro.gyro.z;
    
    // Add the acceleration raw data to the gaussian's datasets to make a new mean 
    x_acceleration_average+=x_acceleration; //* Add to the x_accel dataset 
    y_acceleration_average+=y_acceleration; //* Add to the y_accel dataset 
    z_velocity_average+= z_velocity;         //* Add to the z_velocity dataset

    // Process the new gaussian average with the new datapoints added to their respective datasets 
    x_acceleration_average.process();
    y_acceleration_average.process();
    z_velocity_average.process();
    
    // Calculate the derivative for the x_acceleration and update the last_check variable for next iteration 
    derivative = x_acceleration_average.mean - last_check;
    last_check = x_acceleration_average.mean;
}

void setup(void) {

    Serial.begin(115200);
    WiFi.begin(ssid, password);

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

    // Get the data from the accelerometer
    compute_acceleration();
    Data_Set<double,100> angle_dataset; //* Will store the actual values for  theta

    /* 
        If the angular velocity is zero and the x_acceleration is outside the "Normal" thrashold, 
        count as a step and calculate the angle 
    */
   if (Data_Analysis::inside_thrashold(z_velocity_average.mean, z_velocity_thrashold) && !Data_Analysis::inside_thrashold(x_acceleration_average.mean, x_acceleration_thrashold)){       

        // Some times it returns multiple values for one step, so save them and only utilize the maximun one
        while ( Data_Analysis::inside_thrashold(z_velocity_average.mean, z_velocity_thrashold) && !Data_Analysis::inside_thrashold(x_acceleration_average.mean, x_acceleration_thrashold)){
            
            //!! To avoid counting outliers, only register values inside the domain of the arccos function (i.e -1,1) //!!
            double cos_theta = y_acceleration_average.mean/9.81;
            if (cos_theta <= 1 && cos_theta >= -1){

                angle_dataset.push_back(cos_theta);
            }

            // Get the new datapoinst from the gyro
            compute_acceleration();
  
     }

        // Now we can take the max value of the cos and find the angle 
       HTTPClient http;
       http.begin("http://192.168.15.3:5000/send_data/data="+String(std::acos(angle_dataset.max_value()))); //Specify the URL
       int http_code = http.GET();

        log_ln("http://192.168.15.3:5000/send_data/data="+String(std::acos(angle_dataset.max_value())));
        log("Theta:");
        log_ln(std::acos(angle_dataset.max_value()));

        // Notify the App that there was a step

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

     log("z_velocity:");
     log(z_velocity_average.mean);
     log(",");

     log("derivative:");
     log_ln(derivative);
     
    
}
