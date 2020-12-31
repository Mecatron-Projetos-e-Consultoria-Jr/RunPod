// Include the necessary libraries 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


/// WiFi webserver connection /// 
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VIVO-6464";
const char* password = "C6624A6464";



// Macros during Development
#define log(x) Serial.print(x)
#define log_ln(x) Serial.println(x)


// Instantiate the accelerometer class
Adafruit_MPU6050 mpu;


// Variables used to preserve the gyro data from one iteration to the next
double x_acceleration = 0; //* Raw data from the accelerometer on the x axis
double y_acceleration = 0; //* Raw data from the accelerometer on the y axis
double z_velocity = 0;     //* Raw data from the gyroscope




void compute_acceleration(){

    /* Get new sensor events with the readings */
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    x_acceleration = accel.acceleration.x;
    y_acceleration = accel.acceleration.y;
    z_velocity = gyro.gyro.y; // y for feet

    
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
    
    // Send the data to the local webserver 
    HTTPClient http;
    http.begin("http://190.168.3/send_data/x_acce="+String(x_acceleration)+"/y_acce="+String(y_acceleration)+"/z_vel="+String(z_velocity));
    int status_code = http.GET();
     
}
