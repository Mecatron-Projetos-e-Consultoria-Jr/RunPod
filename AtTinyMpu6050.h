#pragma once
#include "TinyWireM.h"
#include <stdio.h>

class AtTinyMpu6050
{
public:
    class RawData
    {
    public:
        double x_acceleration, y_acceleration, z_acceleration;
        double x_velocity, y_velocity, z_velocity;
    };

    // Initial setup necessary to read data from the MPU6050 via I2C
    void Begin();

    // Get the raw data in the form of a RawData instance
    AtTinyMpu6050::RawData GetRawData();

    // Set which is the I2C Address for the MPU6050, default is 0x6B
    void SetAddress(char address);

    // Wrapper class for the rawData returned by the MPU6050

private:
    char registry_memory_ = 0x68;
};
