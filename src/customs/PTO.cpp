#include "main.h"

#define LEFT_LIFT 3
#define RIGHT_LIFT 3

bool pto_piston_enabled = false;  // Current PTO state

// Toggle motors from PTO, toggle piston, switch brake modes
void pto_toggle_lift(bool toggle) {
  pto_piston_enabled = toggle;
  ezchassis.pto_toggle({ezchassis.left_motors[LEFT_LIFT],ezchassis.right_motors[RIGHT_LIFT]}, toggle);
  pto_lift_piston.set(toggle);
  if (toggle) {
    ezchassis.left_motors[LEFT_LIFT].set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    ezchassis.right_motors[RIGHT_LIFT].set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  }
};

// Global intake function (for use outside of this file)
void set_lift(int input) {
  pto_toggle_lift(input == 0 ? false : true);
  if (!pto_piston_enabled) return;
  ezchassis.left_motors[LEFT_LIFT] = -input;
  ezchassis.right_motors[RIGHT_LIFT] = -input;
}

// User control code
void lift_control() {
  if (master.get_digital(DIGITAL_L1)) {
    set_lift(127);
  }
  else if (master.get_digital(DIGITAL_L2)) {
    set_lift(-127);
  }
  else {
    set_lift(0);
  }
}