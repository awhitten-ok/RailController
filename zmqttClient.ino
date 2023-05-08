#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "stepperController.h"



void clientConnect() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
const char* pTopic = "DRK2/response";
const char* sTopic = "DRK2/command";
  const char* user = "DRK2";


  //Loops until WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    yield();
  }
  Serial.println("Connected to the WiFi network");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  //Initialize Mqtt Client
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //Loops until Connected to Mqtt Broker/Server
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(user, mqttUser, mqttPassword, pTopic,0,false,"-1")) { //Change ClientID for each rail

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);

    }
    //delay(0);
    yield();
  }
  

  client.subscribe(sTopic);
  yield();
}

//Callback fuction for mqtt client 
//All Communication Logic goes here
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  //Serial.println(topic);

  Serial.print("Message:");
  String tempMessage = payloadToString(payload, length);
  //Serial.print(tempMessage);

  Serial.println();
  Serial.println("-----------------------");
  // CHANGE TO SWITCH STATEMENT
  if (tempMessage.equals("moveToPosition0")) {
    stepControl.moveToPosition(stepControl.getHomePosition(), client);
  } else if (tempMessage.equals("moveToPosition1")) {
      stepControl.moveToPosition(stepControl.getLockPosition(0), client);
      //Serial.println("Error after callback call");
      yield();
  }else if (tempMessage.equals("moveToPosition2")) {
      stepControl.moveToPosition(stepControl.getLockPosition(1), client);
      //Serial.println("Error after callback call");
      yield();
  }else if (tempMessage.equals("moveToPosition3")) {
      stepControl.moveToPosition(stepControl.getLockPosition(2), client);
      //Serial.println("Error after callback call");
      yield();  
  }else if (tempMessage.equals("moveToPosition4")) {
      stepControl.moveToPosition(stepControl.getLockPosition(3), client);
      //Serial.println("Error after callback call");
      yield();  
  }else if (tempMessage.equals("moveToPosition5")) {
      stepControl.moveToPosition(stepControl.getLockPosition(4), client);
      //Serial.println("Error after callback call");
      yield();  
  }else if (tempMessage.equals("moveToPosition6")) {
      stepControl.moveToPosition(stepControl.getLockPosition(5), client);
      //Serial.println("Error after callback call");
      yield();  
  }else if (tempMessage.equals("openLock")) {
    stepControl.openLock(client);
    yield();
  } else if(tempMessage.equals("areYouReady")){
    client.publish(pTopic, "99");
    yield();
  } else if(tempMessage.equals("IPhone")){
    stepControl.setOffSet(-1000);
    client.publish(pTopic, "IPhone");
  } else if(tempMessage.equals("Android")){
    stepControl.setOffSet(0);
    client.publish(pTopic, "Android");
  }
  yield();
}

//Converts MqttMessage payload to String
String payloadToString(byte* payload, unsigned int length) {
  String message = "";
  unsigned int i = 0;
  for (i = 0; i < length; i++) {
    message = message + (char)payload[i];
    
    yield();
  }
  //free(payload);
  message = message + '\0';
  Serial.println(message);
  yield();
  return message;
}
void Restart() {
  ESP.restart();
  yield();
}
