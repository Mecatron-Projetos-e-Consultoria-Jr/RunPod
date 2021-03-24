#include "MPU6050_RAW.h"

void Mpu6050::Begin() {
    Wire.begin();
    Wire.beginTransmission(registry_memory_);
    Wire.write(0x6B);        //  Power setting address
    Wire.write(0b00000000);  // Disable sleep mode (just in case)
    Wire.endTransmission();
    Wire.beginTransmission(registry_memory_);
    Wire.write(0x1B);        // Config register for Gyro
    Wire.write(0x00000000);  // 250° per second range (default)
    Wire.endTransmission();
    Wire.beginTransmission(registry_memory_);  //I2C address of the MPU
    Wire.write(0x1C);                          // Accelerometer config register
    Wire.write(0b00000000);                    // 2g range +/- (default)
    Wire.endTransmission();
}

Mpu6050::RawData Mpu6050::GetRawData() {
    Mpu6050::RawData data;
    double temp;  //Make it a local variable since we don't need it

    Wire.beginTransmission(registry_memory_);  //I2C address of the MPU
    Wire.write(0x3B);                          //  Acceleration data register
    Wire.endTransmission();
    Wire.requestFrom(registry_memory_, 14);  // Get 6 bytes, 2 for each DoF
    data.x_acceleration = Wire.read() << 8 | Wire.read();
    data.y_acceleration = Wire.read() << 8 | Wire.read();
    data.z_acceleration = Wire.read() << 8 | Wire.read();
    temp = Wire.read() << 8 | Wire.read();             // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    data.x_velocity = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    data.y_velocity = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    data.z_velocity = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    return data;
}

void Mpu6050::SetAddress(char address) {
    registry_memory_ = address;
}