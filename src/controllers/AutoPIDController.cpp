#include "AutoPIDController.h"

AutoPIDController::AutoPIDController()
    :
    gyro(0),
    lEncoder(1, 2),
    rEncoder(3, 4),

    // Angular PID constants
    kPa(0.1),
    kIa(0.0),
    kDa(0.0),

    // Distance PID constants
    kPd(0.1),
    kId(0.0),
    kDd(0.0),

    // Motor PID constants
    kPw(0.1),
    kIw(0.0),
    kDw(0.0),
    
    aController(kPa, kIa, kDa, &gyro, 0),
    dController(kPd, kId, kDd, &lEncoder, 0), // Distance controller should take input from an arbitrary wheel
    wlController(kPw, kIw, kDw, &lEncoder, 0),
    wrController(kPw, kIw, kDw, &rEncoder, 0),
    stepZeroDistance(10),
    stepOneAngle(7.0),
    stepTwoDistance(8)

{
    aController.SetPIDSourceType(PIDSourceType::kDisplacement); // Control angle, not angular velocity
    dController.SetPIDSourceType(PIDSourceType::kDisplacement); // Control distance, not velocity.
    wlController.SetPIDSourceType(PIDSourceType::kRate); // Control turning rate
    wrController.SetPIDSourceType(PIDSourceType::kRate); // Control turning rate.

    aController.SetContinuous(true);
    dController.SetContinuous(false);

    gyro.InitGyro();
}
//! Autonomously handle the robot.

/**
* Using sensors and PID control, score points without driver input.
* Current autonomous plan is to put the shooter down,
* Go under the low bar, turn, and outtake a ball into the low goal.
* @param bot The Robot to control.
*/
void AutoPIDController::handle(SlothRobot* bot)
{
    float speed = dController.Get(); //!< The base speed of the robot wheels
    float angle = aController.Get(); //!< The difference in wheel speeds between left and right.
    wlController.SetSetPoint(speed+angle);
    wrController.SetSetpoint(speed-angle);
    // TODO: May need to reverse direction of error correction.
    bot->drivetrain.DriveLR(speed+angle+wlController.Get(), speed-angle+wrController.Get());
    
}
//! This will be called at AutonomousInit(), so that we can keep track of what we have to do.
void AutoPIDController::start()
{
    gyro.Calibrate();
    aController.SetSetpoint(0.0);
    dController.SetSetpoint(stepZeroDistance);

    aController.Enable();
    dController.Enable();
    wlController.Enable();
    wrController.Enable();

    std::cout << "Starting Autonomous Control." << std::endl;
}
