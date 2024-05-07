#include "main.h"

void SoloAWP(){
    chassis.setPose(-51, -33.5, 112);
    chassis.moveToPose(-10,-40,90,1000);
    chassis.moveToPose(-20,-40,90,1000);
    chassis.turnToHeading(0, 250);
    chassis.moveToPose(-47, -47, 225, 1500);
};
