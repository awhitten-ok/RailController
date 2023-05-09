#include <ESP8266WiFi.h>
#include <PubSubClient.h>



#include "stepperController.h"
const char* ssid = "OpenKey_Wifi";
const char* password = "OpenKey is #1 in keys!";
//const char* ssid = "WhittenWiFi";
//const char* password = "Amwhitten2016";
const char* mqttServer = "192.168.1.152";
const char* mqttUser = NULL;
const char* mqttPassword = NULL;
int mqttPort = 1883;
const char* pTopic = "DRK3/response";
const char* sTopic = "DRK3/command";

WiFiClient espClient;
PubSubClient client(espClient);
stepperController stepControl;


void setup() {

  Serial.begin(115200);
  stepControl.stepperSetup();
  //pinMode(D3, HIGH);
  clientConnect();
}
void loop() {
 
  if (stepControl.getHasRun() == 0) {//Reset to 0 for actual test
   stepControl.calibration();
   client.publish(pTopic, "99");
   yield();
  }
  if (client.connected() == false){
    clientConnect();
    yield();
  }
  client.loop();
  yield();
  delay(500);
}
