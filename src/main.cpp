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
bool cataInTheZone = false;

void initalize(void){
  vexcodeInit();
  Brain.Screen.print("initalization");
  wings.set(true); 
}

void setChassisVelocity(float numPercent){
  LeftMotor.setVelocity(numPercent, rpm);
  RightMotor.setVelocity(numPercent, rpm);
}
void driveDistanceInches(float distance){
  setChassisVelocity(200);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void turnChassisLeft(float numTurns){
  setChassisVelocity(100);
  LeftMotor.spinFor(reverse, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(forward, numTurns, turns);
}
void turnChassisRight(float numTurns){
  setChassisVelocity(100);
  LeftMotor.spinFor(forward, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(reverse, numTurns * turn_multiplier, turns);
}
void toggle_wings(){
  wings.set(!wings.value());
  wait(1000, msec);
}

void toggle_cata() {
  isCataRunning = !isCataRunning;
  cataInTheZone = false;
  wait(1000, msec);
}
void preauton(void){
  Brain.Screen.print("Preauton"); 
  wings.set(true); 
  setChassisVelocity(200);
  Intake.setVelocity(50, percent);
}
void practiceauton(){ // No longer the main auton
  Intake.spinFor(reverse, 1, sec);
  turnChassisRight(1);
  Intake.spinFor(forward, 1, sec);
  driveDistanceInches(19);
  wait(750, msec);
  turnChassisLeft(1);
  driveDistanceInches(10);
}
void auton(){
  Intake.spinFor(reverse, 1, sec);
  turnChassisRight(1);
  Intake.spinFor(forward, 1, sec);
  driveDistanceInches(38); // Pushing the preload into the goal
  wait(500,msec);
  driveDistanceInches(-10);
  turnChassisLeft(2.7);
  driveDistanceInches(20);
  turnChassisRight(2);
  wings.set(false);
  driveDistanceInches(33);
  turnChassisRight(2);
  wait(250,msec);
  driveDistanceInches(33);
  wait(250, msec);
  wings.set(true);
  driveDistanceInches(-16.5);
}
void auton_2(){
  Intake.spinFor(reverse, 1, sec);
  turnChassisRight(2);
  Intake.spinFor(forward, 1.25, sec);
  driveDistanceInches(30.75);
}
void auton_skills(){ // Autonomous skills section
  driveDistanceInches(26.04); // Move forward
  wings.set(false); // Open wings
  wait(750, msec); // Wait
  turnChassisRight(2); // turn right towards pvc pipe
  driveDistanceInches(12); // move towards the pvc pipe
  turnChassisLeft(2); // turn chassis left, left of the pvc pipe
  driveDistanceInches(48); // move robot forwards
  turnChassisRight(2); //  turn right, towards the pvc
  driveDistanceInches(5); // slightly move, to push triballs over the pvc
  turnChassisLeft(2); // turn left, back towards the same position
  wings.set(true); // close wings
  wait(750, msec);
  turnChassisLeft(2); // turn robot left
  driveDistanceInches(36); // move forward
  turnChassisRight(2); // turn towards the top
  driveDistanceInches(23); // move towards the top
  turnChassisRight(2); // turn right
  driveDistanceInches(24);// move under the elevation bar
  turnChassisRight(2); // turn right
  driveDistanceInches(26.04); // Move forward
  wings.set(false); // Open wings
  wait(750, msec); // Wait
  turnChassisRight(2); // turn right towards pvc pipe
  driveDistanceInches(12); // move towards the pvc pipe
  turnChassisLeft(2); // turn chassis left, left of the pvc pipe
  driveDistanceInches(25); // move robot forwards
  turnChassisLeft(2); // turn robot left
  driveDistanceInches(35);
  wait(750, msec);
}
void hiddencommands(){ // Temporary notatiobn that shows the hidden auton functions
  auton();
  auton_2();
}
void driver_control(){
  int deadband = 5;
  Intake.setVelocity(50, percent);
  Catapult.setVelocity(40, rpm);
  Catapult.setPosition(0, deg);
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

    //scenario 1: cata is running + not in reset pos - cata spins forward
    // scenario 2: cata is running + in reset pos - cata spins forward
    // scenario 3: cata is not running + not in reset pos - cata spins forward
    // scenario 4: cata is not running + in reset pos - cata stops
    
    if(cataInTheZone) {
      Catapult.stop();
    } else if ((!(int) Catapult.position(deg) % 180 >= 150 && (int) Catapult.position(deg) % 180 <= 152)) {
      Catapult.spin(forward);
      Brain.Screen.print("in position");
    } else if (isCataRunning) {
      Catapult.spin(forward);
    } else {
      cataInTheZone = true;
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
  Competition.autonomous(practiceauton);
  Competition.drivercontrol(driver_control);
  preauton();
}
