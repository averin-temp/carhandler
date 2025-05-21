// CarHandler.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <csignal>
#include "Application.h"
#include "ControlComponent.h"

#include "MainComponent.h"
#include "BeaconComponent.h"


Application* pApp = nullptr;

void signalHandler(int signum) 
{
    if (pApp != nullptr)
        pApp->stop();
}


int main()
{
    signal(SIGPIPE, SIG_IGN);
    
    Application app;
    pApp = &app;
    signal(SIGINT, signalHandler);

    app.controlEnable = true;
    MainComponent m(pApp);
    BeaconComponent b(pApp);
    app.addComponent(&m);
    app.addComponent(&b);
    ControlComponent c(pApp);
    app.addComponent(&c);
    app.run();
}