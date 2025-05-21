#pragma once


class ICarDriver {
public:
    virtual void commandForward() = 0;
    virtual void commandBackward() = 0;
    virtual void commandStop() = 0;
    virtual void commandTurnLeft() = 0;
    virtual void commandTurnRight() = 0;
    virtual void commandTurnCenter() = 0;
    virtual void calibrate() = 0;
    virtual ~ICarDriver() = default;
};



