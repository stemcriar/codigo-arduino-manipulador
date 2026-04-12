#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "ServoMotor.h" 

class Manipulator {
  private:
    ServoMotor * servos;

  public:
    Manipulator();

    void setPins(int * pins, int lenght);
    void setPositions(int * positions, int lenght);
    
    void online(int servo, int destiny);
    void offline(int * positions);

    int getServoAngle(int servo);

    ~Manipulator();

};

#endif
