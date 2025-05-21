#pragma once

#include "Command.h"
#include "ICarDriver.h"

class CarController
{
    private:
    Command state;
    ICarDriver* driver;

public:
    CarController();
    ~CarController();

    void setState(Command* command);
    void reset();
};