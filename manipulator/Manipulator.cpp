#include "Manipulator.h"

Manipulator::Manipulator()
{
  this->servos = new ServoMotor[5] 
  {
        ServoMotor("Base"),
        ServoMotor("Braço"),
        ServoMotor("Antibraço"),
        ServoMotor("Punho"),
        ServoMotor("Garra")
  };
};

void Manipulator::setPins(int * pins, int lenght) 
{
  for (int i = 0; i < lenght; i++)
  {
    this->servos[i].toAttach(pins[i]);
  }

  delay(250);
};

void Manipulator::setPositions(int * positions, int lenght) 
{
  for (int i = 0; i < lenght; i++)
  {
    this->servos[i].rotate(positions[i]);
  }
};

void Manipulator::online(int servo, int destiny) 
{
  if (servo != 4)
  {
    this->servos[servo].rotate(destiny);
    
    return; 
  }

  if (destiny == 0)
  {
    this->servos[4].rotate(0);
  }
  else
  {
    this->servos[4].rotate(90);
  }
}

int Manipulator::getServoAngle(int servo) 
{
  return this->servos[servo].getAngle();
}

void Manipulator::offline(int * positions) 
{
  for (int i = 0; i < 4; i++)
  {
    int newAngle = this->servos[i].getAngle() + (positions[i] * 2);
    newAngle = constrain(newAngle, 0, 180);

    this->servos[i].rotate(newAngle);
  }

  if (positions[4] == 0)
  {
    this->servos[4].rotate(0);
  }
  else
  {
    this->servos[4].rotate(90);
  }
}


Manipulator::~Manipulator() 
{
  delete[] servos;
};
