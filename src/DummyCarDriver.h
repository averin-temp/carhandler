#pragma once

#include "ICarDriver.h"


class DummyCarDriver : public ICarDriver
{
public:
    DummyCarDriver();
    ~DummyCarDriver();
    void commandForward();
    void commandBackward();
    void commandStop();
    void commandTurnLeft();
    void commandTurnRight();
    void commandTurnCenter();
    void calibrate();
};

