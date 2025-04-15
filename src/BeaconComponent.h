#pragma once

#include "ApplicationComponent.h"
#include "UdpBeacon.h"
#include "IMainComponent.h"


class BeaconComponent : public ApplicationComponent
{
private:
	UdpBeacon* beacon = nullptr;
	IMainComponent* pMainComponents;
public:
	BeaconComponent(Application* app, IMainComponent* pMain);
	void create();
	void destroy();
	void run();
};

