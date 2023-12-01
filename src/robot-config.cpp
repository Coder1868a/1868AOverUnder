#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;
// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller1;
motor rightmotorA = motor(PORT16, ratio6_1, false);
motor rightmotorB = motor(PORT17, ratio6_1, false);
motor rightmotorM = motor(PORT13, ratio6_1, true);
motor_group RightMotor = motor_group(rightmotorA, rightmotorM, rightmotorB);
motor leftmotorA = motor(PORT19, ratio6_1, true);
motor leftmotorM = motor(PORT10,ratio6_1, false);
motor leftmotorB = motor(PORT20, ratio6_1, true);
motor_group LeftMotor = motor_group(leftmotorA, leftmotorM, leftmotorB);
motor Intake = motor(PORT11, ratio6_1, true);
motor Catapult = motor(PORT18, ratio36_1, false);
digital_out wings = digital_out(Brain.ThreeWirePort.A);
bool RemoteControlCodeEnabled = true;
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}