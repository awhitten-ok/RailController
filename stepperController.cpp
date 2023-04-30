#include <PubSubClient.h>
#include <AccelStepper.h>
#include "stepperController.h"
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
  lock1Position = 1298; //Potentially one variable for all 3 locks depending if lock positions are equidistant 
  lock2Position = 3370;
  lock3Position = 5411;
  pTopic = "Assa1/response";
  sTopic = "Assa1/command";


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
long stepperController::getLock1Position() {
  return lock1Position;
}
long stepperController::getLock2Position() {
  return lock2Position;
}
long stepperController::getLock3Position() {
  return lock3Position;
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
  }else if(x == lock1Position){
     message = "1"; //Moved to Lock 1
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lock1Position + offSet);
    yield();
   delay(0);
  }else if(x == lock2Position){
     message = "2"; //Moved to Lock 2
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lock2Position + offSet);
    yield();
   delay(0);
  }else if(x == lock3Position){
     message = "3"; //Moved to Lock 3
     Serial.println(stepper.maxSpeed());
    stepper.moveTo(lock3Position + offSet);
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
  
  if (stepper.currentPosition() == x + offSet) {//Checks if the move was succesful.
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
