#ifndef stepperController_h
#define stepperController_h
#include <PubSubClient.h>
#include <AccelStepper.h>

class stepperController {
  private:
    AccelStepper stepper;
    const int StepsAcrossRail = 500;
    const int Enable = 13; // Create a variable holding enable pin address
    const int Prox = 4; // Create variable holding proximity sensor pin address
    const int Lock = 5; // Create variable holding microstepping pin address
    const int Switch = 15;
    int SwitchState;
    int ProxState;
    int hasRun;
    int LockState;
    int offSet;
    long homePosition;
    long lock1Position;
    long lock2Position;
    long lock3Position;
    char* pTopic;
    char* sTopic;


  public:
    stepperController();
    void stepperSetup();
    void moveToPosition(long pos, PubSubClient client);
    void openLock(PubSubClient client);
    void calibration();
    int getStepsAcrossRail();
    int getEnable();
    int getProx();
    int getLock();
    int getSwitch();
    int getSwitchState();
    int getProxState();
    int getLockState();
    int getOffSet();
    int getHasRun();
    AccelStepper getStepper();
    long getHomePosition();
    long getLock1Position();
    long getLock2Position();
    long getLock3Position();
    void  setOffSet(int x);
    void setProxState(int state);
    void setLockState(int state);
    void setHasRun(int state);
};
#endif
