#pragma once
#include <WPILib.h>

class Intake {
private:
    CANTalon arm;
    CANTalon roller;
public: 
    Intake();
    void SetArm(double value);
    void SetRoller(double value);
};
