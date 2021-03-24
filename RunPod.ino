#include <Wire.h>

#include "BLE.h"

Bluetooth::BLE ble;

void setup() {
    Serial.begin(9600);

    // Set Up BLE
    ble.setDeviceName("Pedro's RunPod");
    ble.add_characteristic("Raw", "fbed8ddc-109f-11eb-adc1-0242ac120002");
    ble.use_characteristic("Raw");
}

void loop() {
}