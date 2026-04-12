#include "Manipulator.h"
#include "Control.h"
#include "serial_comm.h" 

#define LENGHT 5
#define OPTION 9

Serial_comm serial_comm; 

Manipulator manipulator;
Control control;

bool option;

int pins[LENGHT] = {4, 5, 6, 7, 8};
int initialPositions[LENGHT] = {90, 60, 50, 140, 0};

int * positions;

unsigned long previousMillis = 0;
const long interval = 50;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(OPTION, INPUT);

  if (digitalRead(OPTION) != LOW) {
    serial_comm.setHandshakeInterval(1000);
    serial_comm.setType("manipulador"); 
    serial_comm.waitHandshake("ARD", serial_comm.getType(), "ESP", "OK");
  }

  manipulator.setPins(pins, LENGHT);
  manipulator.setPositions(initialPositions, LENGHT);
  delay(1000);
}

void loop()
{
  option = digitalRead(OPTION);
  
  if (option != LOW) { // MODO ONLINE
    
    serial_comm.getJson();
    
    if (serial_comm.jsonUpdateCheck()) 
    {
      String origem = serial_comm.from;
      int angulo = serial_comm.angle;

      if (angulo >= 0 && angulo <= 180) {
        if (origem == "base") {
          manipulator.online(0, angulo);
        }
        else if (origem == "braco") {
          manipulator.online(1, angulo);
        }
        else if (origem == "antebraco") {
          manipulator.online(2, angulo);
        }
        else if (origem == "punho") {
          manipulator.online(3, angulo);
        }
        else if (origem == "garra") {
          manipulator.online(4, angulo);
        }
      }
    }
  }
  else // MODO OFFLINE
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      positions = control.controlPositions(A0, A1, A2, A3, 2);
    }
    manipulator.offline(positions);
  }

}
