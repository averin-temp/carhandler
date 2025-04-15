// CarHandler.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <csignal>
#include "Application.h"
#include "StreamComponent.h"
#include "BeaconComponent.h"
#include "MainComponent.h"

Application* pApp = nullptr;

void signalHandler(int signum) 
{
    if (pApp != nullptr)
        pApp->stop();
}


int main()
{
    Application app;
    pApp = &app;
    signal(SIGINT, signalHandler);

    StreamComponent* strCmp = new StreamComponent(pApp);
    MainComponent* mainCmp = new MainComponent(pApp, strCmp);
    BeaconComponent* beaconCmp = new BeaconComponent(pApp, mainCmp);

    app.addComponent(beaconCmp);
    app.addComponent(mainCmp);
    app.addComponent(strCmp);
    app.run();
}