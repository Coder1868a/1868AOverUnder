/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Julia                                                     */
/*    Created:      Thu Nov 09 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
using namespace vex;
competition Competition;
int deadband = 5;
double pi = 3.1416;
double WHEEL_DIAMETER = 4;
double WHEEL_CICUMFERENCE = WHEEL_DIAMETER * pi;
double CHASSIS_GEAR_RATIO = 84.0 / 36.0;
double turn_multiplier = 0.807;
double drive_multiplier = 2.275;
bool isCataRunning = false;
void initalize(void){
  vexcodeInit();
  Brain.Screen.print("initalization");
  wings.set(true); 
}

void setChassisVelocity(float numPercent, float numApprox){
  LeftMotor.setVelocity(numPercent * numApprox, percent);
  RightMotor.setVelocity(numPercent * numApprox, percent);
}
void driveDistanceInches(int base, float distance){
  setChassisVelocity(base, 10/4);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void turnChassisLeft(float numTurns){
  setChassisVelocity(70, 0.85);
  LeftMotor.spinFor(reverse, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(forward, numTurns, turns);
}
void turnChassisRight(float numTurns){
  setChassisVelocity(70, 0.85);
  LeftMotor.spinFor(forward, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(reverse, numTurns * turn_multiplier, turns);
}
void toggle_wings(){
  wings.set(!wings.value());
  wait(1000, msec);
}

void toggle_cata() {
  isCataRunning = !isCataRunning;
  wait(1000, msec);
}
void preauton(void){
  Brain.Screen.print("Preauton"); 
  wings.set(true); 
  setChassisVelocity(40,1);
  Intake.setVelocity(50, percent);
}
void practiceauton(){ // No longer the main auton
  setChassisVelocity(40, 1);
  // driveDistanceInches(7*9.1/4);
  turnChassisLeft(4*0.807);
}
void auton(){
  Intake.spinFor(reverse, 500, msec);
  turnChassisRight(0.5);
  Intake.spinFor(forward, 500, msec);
  driveDistanceInches(50, 32); // Pushing the preload into the goal
  wait(500,msec);
  driveDistanceInches(50, -15);
  turnChassisLeft(3);
  driveDistanceInches(50, 33);
  turnChassisRight(2);
  wings.set(false);
  driveDistanceInches(20, 33);
  turnChassisRight(2);
  wait(250,msec);
  driveDistanceInches(50, 33);
  wait(250, msec);
  wings.set(true);
  driveDistanceInches(50, -16.5);
}
void auton_2(){
  Intake.spinFor(reverse, 1, sec);
  turnChassisRight(2);
  Intake.spinFor(forward, 1.25, sec);
  driveDistanceInches(50, 30.75);
}
void auton_skills(){ // Autonomous skills section
}
void hiddencommands(){
  auton_2();
}
void driver_control(){
  int deadband = 5;
  Intake.setVelocity(50, percent);
  Catapult.setVelocity(40, rpm);
  wings.set(true);
  while(1){
    int leftmotorspeed = Controller1.Axis3.position() + Controller1.Axis1.position();
    int rightmotorspeed = Controller1.Axis3.position() - Controller1.Axis1.position();
    if(abs(leftmotorspeed) < deadband){
      LeftMotor.setVelocity(0, percent);
    } else{
      LeftMotor.setVelocity(leftmotorspeed, percent);
    }
      if(abs(rightmotorspeed) < deadband){
      RightMotor.setVelocity(0, percent);
    } else{
      RightMotor.setVelocity(rightmotorspeed, percent);
    }
    if(Controller1.ButtonL1.pressing()){
      Intake.spin(forward);
    } else if(Controller1.ButtonR1.pressing()){
      Intake.spin(reverse);
    } else{
      Intake.stop();
    }
    if(isCataRunning){
      Catapult.spin(forward);
    } else{
      Catapult.stop();
    }
    LeftMotor.spin(forward);
    RightMotor.spin(forward);
    Controller1.ButtonX.pressed(toggle_wings);
    Controller1.ButtonA.pressed(toggle_cata);
    wait(6,msec);
  }
}
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  initalize();
  Competition.autonomous(auton);
  Competition.drivercontrol(driver_control);
  preauton();
}
