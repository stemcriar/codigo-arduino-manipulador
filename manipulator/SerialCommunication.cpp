#include "SerialCommunication.h"

#define arduino Serial

SerialCommunication::SerialCommunication() {
}

StaticJsonDocument<100> SerialCommunication::getReceivedMsg() const {
  return this->receivedMsg;
}

StaticJsonDocument<100> SerialCommunication::getLastReceivedMsg() const {
  return this->lastReceivedMsg;
}

String SerialCommunication::getLastSentMsg() const {
  return this->lastSentMsg;
}

void SerialCommunication::doHandshake(String data)
{
  // {"Id": "ESP", "Data": "Ok"}
  unsigned long previousMillis = 0;
  
  while (this->receivedMsg["Data"] != "Ok")
  {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis > 1000)
    {
      previousMillis = currentMillis;
      this->send(data);  
    }

    this->receive();
  }

  arduino.println("Handshake received: " + this->receivedMsg["Data"].as<String>() + " from " + this->receivedMsg["Id"].as<String>());
}

void SerialCommunication::waitHandshake(String data)
{
  while (this->receivedMsg["Data"] != "Hello")
  {
    this->receive();
  }

  this->send(data);
  
  arduino.println("Handshake received: " + this->receivedMsg["Data"].as<String>() + " from " + this->receivedMsg["Id"].as<String>());
}

void SerialCommunication::receive()
{
  String data;
  
  if (arduino.available() > 0) 
  {
    data = arduino.readStringUntil('\n');

    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, data);
    
    if (error == DeserializationError::Ok) 
    {
      this->lastReceivedMsg = this->receivedMsg;
      this->receivedMsg = doc;

      this->status = true;
    } 
    else 
    {
      while (Serial.available() > 0){
        Serial.read();
      }
    }
  }
}

void SerialCommunication::send(String msg) {
  this->lastSentMsg = msg;
  arduino.println(msg);
}
