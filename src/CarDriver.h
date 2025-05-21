#pragma once

#include "ICarDriver.h"


class CarDriver : public ICarDriver
{
public:
    CarDriver();
    ~CarDriver();
    void commandForward();
    void commandBackward();
    void commandStop();
    void commandTurnLeft();
    void commandTurnRight();
    void commandTurnCenter();
};

