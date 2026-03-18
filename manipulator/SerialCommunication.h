#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <ArduinoJson.h>

class SerialCommunication
{
  private:
    StaticJsonDocument<100> receivedMsg;
    
    StaticJsonDocument<100> lastReceivedMsg;
    String lastSentMsg;
    
  public:
    bool status;
  
    SerialCommunication();

    StaticJsonDocument<100> getReceivedMsg() const;
    StaticJsonDocument<100> getLastReceivedMsg() const;
    String getLastSentMsg() const;

    void doHandshake(String data);
    void waitHandshake(String data); 

    void receive();
    void send(String msg);
};

#endif
