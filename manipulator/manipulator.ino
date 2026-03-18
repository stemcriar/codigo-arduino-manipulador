#include "Manipulator.h"
#include "Control.h"
#include "SerialCommunication.h"

#define arduino Serial
#define LENGHT 5
#define OPTION 9

SerialCommunication serial;

Manipulator manipulator("MNC020");
Control control;

bool option;

int pins[LENGHT] = {4, 5, 6, 7, 8};
int initialPositions[LENGHT] = {90, 60, 50, 140, 0};

int * positions;

unsigned long previousMillis = 0;
const long interval = 50;

String data = "";

void setup() {
  arduino.begin(9600);

  serial.doHandshake("{\"Id\": \"MNC020\", \"Data\": \"Hello\"}");

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(option, INPUT);

  manipulator.setPins(pins, LENGHT);
  manipulator.setPositions(initialPositions, LENGHT);

  delay(1000);
}

void loop()
{
  option = digitalRead(OPTION);
  
  arduino.println(serial.getReceivedMsg().as<String>());
  
  if (option != LOW) {
    serial.receive();

    if (serial.status) 
    {
      if (serial.getReceivedMsg() != NULL && serial.getReceivedMsg() != serial.getLastReceivedMsg()) 
      {
        int servo = serial.getReceivedMsg()["Part"].as<int>();
        int destiny = serial.getReceivedMsg()["Position"].as<int>();
  
        manipulator.online(servo, destiny);
        serial.status = false;
      }
    }
  }
  else
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      positions = control.controlPositions(A0, A1, A2, A3, 2);
    }

    for (int i = 0; i < 5; i++)
    {
      arduino.print(positions[i]);
      arduino.println();
    }

    manipulator.offline(positions);
  }

}
