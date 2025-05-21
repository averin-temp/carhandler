#pragma once

#include "ApplicationComponent.h"
#include "UdpSocket.h"
#include "CarController.h"

class ControlComponent : public ApplicationComponent
{
private:
    UdpSocket* socket;
    CarController controller;
    bool controlActive;
public:
    ControlComponent(Application* app);
    void create();
    void destroy();
    void run();
};