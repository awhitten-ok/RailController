#include <PubSubClient.h>
#include <AccelStepper.h>
#include "stepperController.h"
                                         //Rail   #
const long lockPositions[15][6] = {
    {1185, 3280, 5761, 181, 2478, 4937}, //DRK1   0
    {1283, 3766, 6006, 356, 2742, 5181}, //DRK2   1
    {817,  3463, 5583, 200, 2505, 4711}, //DRK3   2
    {1272, 2796, 5339, 383, 2377, 4499}, //DRK4   3
    {1093, 3200, 5392, 354, 2450, 4535}, //DRK5   4
    {1503, 3918, 5729, 513, 2995, 4807}, //DRK6   5
    {1245, 3707, 5837, 303, 2728, 4945}, //Salto1 6
    {1017, 3334, 5854, 215, 2430, 4955}, //Miwa1  7
    {1296, 3371, 5475, 309, 2357, 4550}, //DRK7   8
    {1450, 3477, 5456, 493, 2580, 4554}, //Kaba1  9
    {1561, 3606, 5622, 555, 2667, 4712}, //Kaba2  10 
    {1543, 3526, 5578, 655, 2526, 4660}, //Kaba3  11
    {1266, 3346, 5500, 343, 2423, 4530}, //Assa1  12
    {1380, 3496, 5657, 383, 2546, 4623}, //Assa2  13
    {1299, 3406, 5589, 383, 2356, 4552}  //Assa3  14
  };
int RAIL_NUMBER = 2;
#define stepPin 14
#define dirPin 12    // Define a stepper and the pins it will use
#define motorInterfaceType 1
#define GET_VARIABLE_NAME(Variable) (#Variable)
stepperController::stepperController() {
  stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
  SwitchState = 0;
  ProxState = 0;
  LockState = 0;
  offSet = 0;
  hasRun = 0;
  homePosition = 0;//Change to 50?               
  pTopic = "DRK3/response";
  sTopic = "DRK3/command";

}

//Getters and Setters   Not all are neccessary and can be removed
AccelStepper stepperController::getStepper() {
  return stepper;
}
int stepperController::getProxState() {
  return ProxState;
}
int stepperController::getLockState() {
  return LockState;
}
int stepperController::getSwitchState() {
  return SwitchState;
}
int stepperController::getOffSet() {
  return offSet;
}
int stepperController::getHasRun() {
  return hasRun;
}
long stepperController::getHomePosition() {
  return homePosition;
}
long stepperController::getLockPosition(int position) {
  return lockPositions[RAIL_NUMBER][position];
}

void stepperController::setOffSet(int x) {
  offSet = x;
}
void stepperController::setHasRun(int state) {
  hasRun = state;
}
void stepperController::setLockState(int newLockState) {
  LockState = newLockState;
}
void stepperController::setProxState(int newProxState) {
  ProxState = newProxState;
}
//void stepperController::setLock

//Setups Stepper Motor
void stepperController::stepperSetup() {
  Serial.println("stepper sertup");
  stepper.setMaxSpeed(2000); //set max speed the motor will turn (steps/second)
  stepper.setAcceleration(20000); //set acceleration (steps/second^2)
  Serial.println(stepper.maxSpeed());
  pinMode(Enable, OUTPUT); // Configure pin 13 (enable pin) as output
  digitalWrite(Enable, HIGH);
  pinMode(Prox, INPUT); // Configure pin D2 (Prox pin) as input
  pinMode(Lock, INPUT); // Configure pin D1 (Lock Signal) as output
  yield();
}

//Moves sled to desired position, publishes result back to User Client
void stepperController::moveToPosition(long x, PubSubClient client) {
  Serial.println("In moveToPosition");
  digitalWrite(dirPin, LOW);
  digitalWrite(Enable, LOW);
  std::string message;
  if(x == homePosition){
   message = "0"; //Moved to Home Position
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(homePosition);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][0]){
     message = "1"; //Moved to Lock 1
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][0]);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][1]){
     message = "2"; //Moved to Lock 2
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][1]);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][2]){
     message = "3"; //Moved to Lock 3
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][2]);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][3]){
     message = "4"; //Moved to Lock 1
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][3]);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][4]){
     message = "5"; //Moved to Lock 2
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][4]);
    yield();
   delay(0);
  }else if(x == lockPositions[RAIL_NUMBER][5]){
     message = "6"; //Moved to Lock 3
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lockPositions[RAIL_NUMBER][5]);
    yield();
   delay(0);
  }
  yield();
  Serial.println("Going to :");
  Serial.print(x);
  while (stepper.distanceToGo() != 0){ // Moves phone to lock    //TEST THIS
  SwitchState = digitalRead(Switch);
  ProxState = digitalRead(Prox);
  LockState = digitalRead(Lock);

  //Serial.println(stepper.distanceToGo());
  
    if (ProxState == HIGH || SwitchState == HIGH){    //If switch on either side of rail is triggered, run callibration.
      Serial.println("ProxState: ");
      Serial.println(ProxState);
      Serial.println("SwitchState: ");
      Serial.println(SwitchState);
      calibration();
      yield();
      delay(0);
    }
    if(LockState == HIGH){
      //openLock(client);
      client.publish(pTopic, "5");
      yield();
      delay(0);
    }
    stepper.run();
    //Serial.println("Moving to lock");
    yield();
   delay(0);
  }
  
  if (stepper.currentPosition() == x) {//Checks if the move was succesful.
    char const *pub = message.c_str();
    client.publish(pTopic, pub);
    client.publish(pTopic, "99"); //Inform user client that esp is ready for another command
    yield();
    delay(0);
  } 
  else {
    client.publish(pTopic, "-2");//Error Code
    yield();
    delay(0);
  }
  Serial.println(stepper.currentPosition());
  delay(0);
  yield();
}

//Evenutally will read response from lock
void stepperController::openLock(PubSubClient client) { //Ask Aaron About this
  client.publish(pTopic, "Lock opened");
  digitalWrite(Enable, LOW);
  Serial.println("Recieved lock signal");
  digitalWrite(Enable, HIGH);
  yield();
}

//Calibration runs once upon reset of ESP
void stepperController::calibration() {
  digitalWrite(Enable, LOW);
  Serial.println("Spinning Counter-Clockwise...");
  ProxState = digitalRead(Prox);
  Serial.println("ProxState");
  Serial.println(ProxState);
  digitalWrite(dirPin, LOW);
  while (ProxState != HIGH) // view recent arduino forum post section 28 for other motor step options turnMot
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    ProxState = digitalRead(Prox);
    yield(); // keeps the watchdog timer from crashing
  }
  stepper.setCurrentPosition(-50);
  stepper.moveTo(50);
  while (stepper.distanceToGo() != 0) // Moves phone to lock
  {
    stepper.run();
    delay(0);
  }
  Serial.println("Reset Zero Position");
  digitalWrite(Enable, HIGH);
  delay(1000);
  yield();
  hasRun = 1;
}
