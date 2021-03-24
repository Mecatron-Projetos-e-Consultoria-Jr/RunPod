#include <Wire.h>

#include "BLE.h"
#include "MPU6050_RAW.h"

Bluetooth::BLE ble;
Mpu6050 mpu;

void setup() {
    Serial.begin(9600);

    // Set Up BLE
    ble.setDeviceName("Pedro's RunPod");
    ble.add_characteristic("Raw", "fbed8ddc-109f-11eb-adc1-0242ac120002");
    ble.use_characteristic("Raw");

    ble.begin();

    // MPU6050 SetUp
    mpu.Begin();
}

void loop() {
    // Get the data from the MPU6050
    Mpu6050::RawData raw_data = mpu.GetRawData();

    // Format and send the data
    char delimiter = ',';
    char package[18];

    package[0] = '1';
    package[1] = ':';

    dtostrf(raw_data.x_acceleration, 7, 0, &package[2]);
    package[9] = delimiter;

    dtostrf(raw_data.y_acceleration, 7, 0, &package[10]);
    package[17] = '\0';

    // ble_device.println(package);
    ble.sendDataPoint(std::string(package));
    delay(200);

    //
    char package_2[20];
    package_2[0] = '2';
    package_2[1] = ':';

    dtostrf(raw_data.z_acceleration, 7, 0, &package_2[2]);
    package_2[9] = delimiter;

    dtostrf(raw_data.z_velocity, 7, 0, &package_2[10]);
    package_2[17] = '\0';
    ble.sendDataPoint(std::string(package_2));
}