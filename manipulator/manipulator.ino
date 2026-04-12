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
      String estado = serial_comm.state;

      if (origem == "joystickR" || origem == "joystickL") {
        // Parse state "X,Y" — each is -1, 0, or 1
        int commaIndex = estado.indexOf(',');
        int dirX = estado.substring(0, commaIndex).toInt();
        int dirY = estado.substring(commaIndex + 1).toInt();

        if (origem == "joystickL") {
          // Joystick R: Y -> Base (servo 0), X -> Braço (servo 1)
          int newAngle0 = constrain(manipulator.getServoAngle(0) - (dirX * 3), 0, 180);
          int newAngle1 = constrain(manipulator.getServoAngle(1) + (dirY * 3), 0, 180);
          manipulator.online(0, newAngle0);
          manipulator.online(1, newAngle1);
        } else {
          // Joystick L: Y -> Antebraço (servo 2), X -> Punho (servo 3)
          int newAngle2 = constrain(manipulator.getServoAngle(2) - (dirY * 3), 0, 180);
          int newAngle3 = constrain(manipulator.getServoAngle(3) - (dirX * 3), 0, 180);
          manipulator.online(2, newAngle2);
          manipulator.online(3, newAngle3);
        }
      }
      else if (origem == "effector") {
        static bool garraFechada = false;
        garraFechada = !garraFechada;
        int angulo = garraFechada ? 90 : 0;
        manipulator.online(4, angulo);
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
