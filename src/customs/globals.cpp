#include "main.h"

ez::Drive ezchassis (
  // Left Chassis Ports (negative=reverse)
  {13, 12, 11}
  // Right Chassis Ports (negative=reverse)
  ,{-18, -19, -20}
  // IMU Port
  ,10
  // Wheel Diameter
  ,4
  // Cartridge RPM
  ,600
  // External Gear Ratio
  ,75);

pros::Controller master(CONTROLLER_MASTER);

pros::Motor FL(13, MOTOR_GEAR_BLUE); // left_motor_group
pros::Motor ML(12, MOTOR_GEAR_BLUE); // left_motor_group
pros::Motor BL(11, MOTOR_GEAR_BLUE); // left_motor_group
pros::Motor FR(-18, MOTOR_GEAR_BLUE); // right_motor_group
pros::Motor MR(-19, MOTOR_GEAR_BLUE); // right_motor_group
pros::Motor BR(-20, MOTOR_GEAR_BLUE); // right_motor_group

// left motor group
pros::MotorGroup leftdrive({ FL, ML, BL });
// right motor group
pros::MotorGroup rightdrive({ FR, MR, BR });

Piston pto_lift_piston('A');    // PTO piston

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftdrive, // left motor group
                              &rightdrive, // right motor group
                              12.482, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              343, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

pros::Imu imu(10);

// horizontal tracking wheel encoder
pros::Rotation horizontal_encoder(20);
// vertical tracking wheel encoder
pros::Rotation vertical_encoder(19);
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// odometry settings
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(5, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.05 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(5, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.05 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain,
                        lateral_controller,
                        angular_controller,
                        sensors,
                        &throttle_curve, 
                        &steer_curve
);
