#include "AtTinyMpu6050.h"

void AtTinyMpu6050::Begin()
{

    TinyWireM.begin();
    TinyWireM.beginTransmission(registry_memory_);
    TinyWireM.write(0x6B);       //  Power setting address
    TinyWireM.write(0b00000000); // Disable sleep mode (just in case)
    TinyWireM.endTransmission();
    TinyWireM.beginTransmission(registry_memory_);
    TinyWireM.write(0x1B);       // Config register for Gyro
    TinyWireM.write(0x00000000); // 250° per second range (default)
    TinyWireM.endTransmission();
    TinyWireM.beginTransmission(registry_memory_); //I2C address of the MPU
    TinyWireM.write(0x1C);                         // Accelerometer config register
    TinyWireM.write(0b00000000);                   // 2g range +/- (default)
    TinyWireM.endTransmission();
}

AtTinyMpu6050::RawData AtTinyMpu6050::GetRawData()
{
    AtTinyMpu6050::RawData data;
    double temp; //Make it a local variable since we don't need it

    TinyWireM.beginTransmission(registry_memory_); //I2C address of the MPU
    TinyWireM.write(0x3B);                         //  Acceleration data register
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(registry_memory_, 14); // Get 6 bytes, 2 for each DoF
    data.x_acceleration = TinyWireM.read() << 8 | TinyWireM.read();
    data.y_acceleration = TinyWireM.read() << 8 | TinyWireM.read();
    data.z_acceleration = TinyWireM.read() << 8 | TinyWireM.read();
    temp = TinyWireM.read() << 8 | TinyWireM.read();            // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    data.x_velocity = TinyWireM.read() << 8 | TinyWireM.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    data.y_velocity = TinyWireM.read() << 8 | TinyWireM.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    data.z_velocity = TinyWireM.read() << 8 | TinyWireM.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    return data;
}

void AtTinyMpu6050::SetAddress(char address)
{
    registry_memory_ = address;
}
