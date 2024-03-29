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
bool hang_up = false;
void initalize(void){
  vexcodeInit();
  Brain.Screen.print("initalization");
  wings.set(true); 
}
// --------------------------------------------------------------------------------------------
/// HELPER FUNCTIONS HERE ////
// --------------------------------------------------------------------------------------------
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
void driveDistanceInches1(float distance){
  setChassisVelocity(250);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void driveDistanceInches2(float distance){
  setChassisVelocity(150);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void driveDistanceInches3(float distance){
  setChassisVelocity(330);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void Ramming(float distance){
  setChassisVelocity(250);
  float INCHES_PER_DEGREE = WHEEL_CICUMFERENCE / 360;
  float degreesSpin = distance / INCHES_PER_DEGREE;
  LeftMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees, false);
  RightMotor.spinFor(forward, degreesSpin * drive_multiplier, degrees);
}
void turnChassisLeft(float numTurns){
  setChassisVelocity(150);
  LeftMotor.spinFor(reverse, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(forward, numTurns, turns);
}
void turnChassisLeft1(float numTurns) {
  setChassisVelocity(100);
  LeftMotor.spinFor(reverse, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(forward, numTurns * turn_multiplier, turns);
}
void turnChassisRight(float numTurns){
  setChassisVelocity(100);
  LeftMotor.spinFor(forward, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(reverse, numTurns * turn_multiplier, turns);
}

void turnChassisRightSlow(float numTurns){
  setChassisVelocity(30);
  LeftMotor.spinFor(forward, numTurns * turn_multiplier, turns, false);
  RightMotor.spinFor(reverse, numTurns * turn_multiplier, turns);
}
void toggle_wings(){
  wings.set(!wings.value());
  wait(1000, msec);
}
void toggle_hang(){
  hang_up = !hang_up;
  hang.set(hang_up);
  wait(1000, msec);
}
void toggle_cata() {
  isCataRunning = !isCataRunning;
  cataInTheZone = false;
  wait(500, msec);
}
// --------------------------------------------------------------------------------------------
//  AUTONS HERE // 
// --------------------------------------------------------------------------------------------
void preauton(void){
  Brain.Screen.print("Preauton"); 
  wings.set(true); 
  setChassisVelocity(200);
  Intake.setVelocity(50, percent);
}
// AUTONS FOR PID // 
// turn variables //
double ratio = 0.1;
double destination = 90;
double error = destination - inertialSensor.rotation(degrees);
// drive variables // 
double kP =0.025;
double kI = 0;
double kD = 0.1;
double turnkP = 0.0001;
double turnkI = 0.0;
double turnkD = 0.0;
int desiredValue = 200; // in degrees
int desiredTurnValue = 0;
int prevError = 0; // position 20 milliseconds ago
int derivative; // error - prevError (slope of position is speed)
int totalError = 0;
int turnError; // current sensor value - desired value: positional value
int turnPrevError = 0; // position 20 milliseconds ago
int turnDerivative; // error - prevError (slope of position is speed)
int turnTotalError = 0;
float WHEEL_DIAM = 4.125;
float PI = 3.1415;
float GEAR_RATIO = 84.0 / 36.0;
bool resetDriveSensors = false;
// variables modified for use
bool enableDrivePID = true;
// PID Function
int drivePID() {
  while(enableDrivePID) {
    if (resetDriveSensors) {
      resetDriveSensors = false;
      LeftMotor.setPosition(0, degrees);
      RightMotor.setPosition(0, degrees);
    }
    int leftMotorPosition = LeftMotor.position(degrees);
    int rightMotorPosition = RightMotor.position(degrees);
     printf("left motor pos is %i\n", leftMotorPosition);
     printf("right motor pos is %i\n", rightMotorPosition);
    // ------------------
    // lateral movement pid
    // ------------------
    // get average of the motors
    int averagePosition = (leftMotorPosition + rightMotorPosition) / 2;
    //potential
    error = desiredValue - averagePosition;
    // derivative
    derivative = error - prevError;
    // velocity -> position (integral)
    // totalError += error; // compound error 
    double lateralMotorPower = (error * kP + derivative * kD);
    // ------------------
    // TURN PID
    // ------------------
    // get average of the motors
    int turnDifference = leftMotorPosition - rightMotorPosition;
    //potential
    turnError = turnDifference - desiredTurnValue;
    // derivative
    turnDerivative = turnError - turnPrevError;
    // velocity -> position (integral)
    // turnTotalError += turnError; // compound error 
    double turnMotorPower = (turnError * turnkP + turnDerivative * turnkD );
    printf("lateral power is %f\n", lateralMotorPower);
    printf("error is %i\n", error);
    RightMotor.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    LeftMotor.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);
    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20); // don't want this using up all of our cpu
  }
  return 1;
}
void emma_inertial_drive_forward(float target) {
  float x = 0.0;
  float error = target - x;
  float speed = 75.0;
  float accuracy = 0.2;
  float ks = 1.0;
  float yaw = 0.0;
  float lspeed = speed * fabs(error) / error - ks * yaw;
  float rspeed = speed * fabs(error) / error + ks * yaw;
  float pint = 0.01;
  inertialSensor.setRotation(0.0, deg);
  rightmotorA.setRotation(0.0, rev);
  while (fabs(error) > accuracy) {
    printf("error is %f\n", error);
    RightMotor.spin(forward);
    LeftMotor.spin(forward);
    x = rightmotorA.position(rev) * PI * WHEEL_DIAM * pint;
    error = target - x; 
    yaw = inertialSensor.rotation(degrees);
    lspeed = speed * fabs(error) / error - ks * yaw;
    rspeed = speed * fabs(error) / error + ks * yaw;
  }
}
void new_turn(){
  vex::task callTask(drivePID);
  LeftMotor.spin(forward);
  RightMotor.spin(reverse);
  waitUntil(inertialSensor.angle() >= 90);
  LeftMotor.stop();
  RightMotor.stop();
}
void opposite_side_pid(){
  vex::task callTask(drivePID);
  resetDriveSensors =  true;
  // turn 90 degrees right
  LeftMotor.spin(forward);
  RightMotor.spin(reverse);
  printf("error is %f\n");
  waitUntil(inertialSensor.rotation(degrees) >= 90);
  LeftMotor.stop();
  RightMotor.stop();
  wait(400, msec);
  // move forward 20 inches //
  desiredTurnValue = 0;
  desiredValue = (20/(WHEEL_DIAM*PI)*360*GEAR_RATIO);
  wait(400, msec);
}
void pidtest(void){
  vex::task callTask(drivePID);
  resetDriveSensors = true;
  desiredValue = (20/(WHEEL_DIAM*PI)*360*GEAR_RATIO);
  desiredTurnValue = 0;
  wait(3000,msec);
  LeftMotor.spin(forward);
  RightMotor.spin(reverse);
  waitUntil(inertialSensor.rotation(degrees) >= 90);
  LeftMotor.stop();
  RightMotor.stop();
  wait(400, msec);
}
void skills_auton() { // basic all match loading skills
  Intake.spinFor(forward, 1, sec);
  turnChassisRight(1);
  driveDistanceInches3(18);
  wait(100, msec);

  turnChassisLeft(0.75);
  driveDistanceInches3(7);
  wait(100,msec);

  turnChassisRight(2); // turn 90 degrees to face match load place
  wings.set(false); // pop wings

  driveDistanceInches2(3); // drive forward into goal
  turnChassisRight(0.25); // adjust for ideal angle
  Catapult.setVelocity(40,rpm);
  Catapult.spinFor(50, sec);
}



void practiceauton(void){ // No longer the main auton
  Intake.spinFor(forward, 0.25, sec);
  turnChassisRight(1);
  // Intake.spinFor(reverse, 1, sec);
  driveDistanceInches(25);
  wait(750, msec);
  turnChassisLeft(0.75);
  driveDistanceInches(14);
  wait(500, msec);
  Ramming(-10);
  Intake.spinFor(forward, 1, sec);
  Ramming(10);
  wait(250, msec);
  Ramming(-10);
  Ramming(10);
  wait(250, msec);
  Ramming(-10);
  Ramming(10);
  wait(250, msec);
  Ramming(-10);
  Intake.spinFor(forward, 1, sec);
}
void emmas_auton() {
  Intake.spinFor(forward, 0.25, sec);
  turnChassisRight(1);
  // Intake.spinFor(reverse, 1, sec);
  driveDistanceInches(25);
  wait(100, msec); // Emma: made this wait a little shorter
  turnChassisLeft(0.75);
  driveDistanceInches3(14);
  wait(300, msec);
  Ramming(-10);
  Intake.spinFor(forward, 0.5, sec); // Emma: made the intake spin less

  turnChassisLeft(1.675); // turn left to balls
  wait(100, msec); // Emma: decreased wait times on mostly everything
  driveDistanceInches3(45); // approach first ball
  Intake.spinFor(reverse, 1.3, sec); // intake ball
  wait(100, msec);
  turnChassisRight(1.5); // turn 90 degree right to face other ball

  driveDistanceInches3(18); // drive to other ball
  //Intake.spinFor(reverse, 1, sec); // intake other ball
  wait(100, msec);
  turnChassisRight(2); // face goal
  wait(100,msec);
  Intake.spinFor(forward, 1, sec);
  wings.set(false);
  driveDistanceInches3(55); //drive into goal!
  

}
void auton(){
  Intake.spinFor(forward, 0.25, sec); // let down intake to be in size
  turnChassisRight(1); // turn 45 degreees
  driveDistanceInches(25); 
  wait(100, msec); // Emma: made this wait a little shorter
  turnChassisLeft(0.75);
  driveDistanceInches(14); // drive to goal and push ball in
  wait(500, msec);
  Ramming(-10); // drives backward from goal
  Intake.spinFor(forward, 0.50, sec); // Emma: made the intake spin less
  turnChassisLeft(1.675); // turn left and outward to the rest of the balls
  wait(100, msec); // Emma: decreased wait times on mostly everything
  driveDistanceInches1(23); // drive to the balls
  wait(25, msec);
  wings.set(false); // pop wings out
  driveDistanceInches2(16); // continue driving to begin to push them in
  turnChassisRight(1.365); // turn 90 degrees
  driveDistanceInches2(24); // drive and continue to corral balls
  wait(100, msec); // add a wait before the turn to allow the balls to settle
  turnChassisRightSlow(2.2); // Emma: i made this slower and turn a little more so the balls hopefully won't roll away
  driveDistanceInches1(55); // drive into goal with wings popped!
}
void auton_2(){
  driveDistanceInches3(-23);
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
  driveDistanceInches(18); // move towards the pvc pipe
  turnChassisLeft(2); // turn chassis left, left of the pvc pipe
  driveDistanceInches(25); // move robot forwards
  turnChassisLeft(2); // turn robot left
  driveDistanceInches(40);
  wait(750, msec);
}
void hiddencommands(){ // Temporary notatiobn that shows the hidden auton functions
  auton();
  auton_2();
  skills_auton();
  /*
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
  driveDistanceInches(18); // move towards the pvc pipe
  turnChassisLeft(2); // turn chassis left, left of the pvc pipe
  driveDistanceInches(25); // move robot forwards
  turnChassisLeft(2); // turn robot left
  driveDistanceInches(40);
  wait(750, msec);
}
  */
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

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(100, 150, "string", (Catapult.position(deg)));
    printf("%f\n", rotationSensor.angle(deg));
    if(cataInTheZone) {
      Catapult.stop();
      Brain.Screen.printAt(100, 100, "im in the zone!");
    } else if (!((int) rotationSensor.angle(deg) % 180 >= 0 && (int) rotationSensor.angle(deg) % 180 <= 5)) {
      Catapult.spin(forward);
      Brain.Screen.printAt(100, 100, "in position");
    } else if (isCataRunning) {
      Catapult.spin(forward);
      Brain.Screen.printAt(100, 100, "im running");
    } else {
      Brain.Screen.printAt(100, 100, "im  stooped");
      cataInTheZone = true;
      Catapult.stop();
    }

    LeftMotor.spin(forward);
    RightMotor.spin(forward);

    Controller1.ButtonX.pressed(toggle_wings);
    Controller1.ButtonA.pressed(toggle_cata);
    Controller1.ButtonUp.pressed(toggle_hang);
    wait(6,msec);
  }
}
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  initalize();
  Competition.autonomous(skills_auton);
  Competition.drivercontrol(driver_control);
  preauton();
  // preventing main from exiting with an infinite loop
  while(true){
    wait(250,msec);
  }
}