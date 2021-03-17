#include "AtTinyMpu6050.h"
#include "SoftwareSerial.h"

SoftwareSerial ble_device(3, 1);  // BLE TX-> ATtiny85 PB0, BLE RX-> ATtiny85 PB1
AtTinyMpu6050 mpu;

void setup() {
    // Begin the MPU6050 with the default values
    mpu.Begin();

    // Start the BLE module
    ble_device.begin(9600);  // start BEL device
    delay(500);              // wait until BLE device starts

    ble_device.println("AT+NAMERunPod");  // change device name
    delay(500);                           // wait for change

    ble_device.println("AT+RESET");  // reset module to enact name change
    delay(1000);                     // wait for reset
}
void loop() {
    // Get the data from the MPU6050
    AtTinyMpu6050::RawData raw_data = mpu.GetRawData();

    char delimiter = ',';
    char package[18];

    package[0] = '1';
    package[1] = ':';
    
    dtostrf(raw_data.x_acceleration, 7,0, &package[2]);
    package[9] = delimiter;

    dtostrf(raw_data.y_acceleration, 7,0, &package[10]);
     package[17] = '\0';

    ble_device.println(package);
    delay(200);

//
    char package_2[20];
    package_2[0] = '2';
    package_2[1] = ':';
    
    dtostrf(raw_data.z_acceleration, 7,0, &package_2[2]);
    package_2[9] = delimiter;

    dtostrf(raw_data.z_velocity, 7,0, &package_2[10]);
    package_2[17] = '\0';
    
    ble_device.println(package_2);
    delay(200);

    
//    dtostrf(raw_data.x_velocity, 7,0, &potato[22]);
//    potato[28] = ',';
//
//    dtostrf(raw_data.y_velocity, 7,0, &potato[29]);
//    potato[35] = ',';
//


    
//    potato=std::string(raw_data.x_acceleration);
//    potato+=String(raw_data.x_acceleration)+d+String(raw_data.y_acceleration)+d+String(raw_data.z_acceleration)+ d + String(raw_data.x_velocity)+d+String(raw_data.y_velocity)+d+String(raw_data.z_velocity);

//    ble_device.println(package);
    // ble_device.print("x_accel:");
    // ble_device.println(raw_data.x_acceleration);
    // delay(100);

    // ble_device.print("y_accel:");
    // ble_device.println(raw_data.y_acceleration);
    // delay(100);

    // ble_device.print("z_accel:");
    // ble_device.println(raw_data.z_acceleration);
    // delay(100);

    // ble_device.print("x_v:");
    // ble_device.println(raw_data.x_velocity);
    // delay(100);

    // ble_device.print("y_v:");
    // ble_device.println(raw_data.y_velocity);
    // delay(100);

    // ble_device.print("z_v:");
    // ble_device.println(raw_data.z_velocity);
}
