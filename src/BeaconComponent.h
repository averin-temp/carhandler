#pragma once

#include "ApplicationComponent.h"
#include "UdpBeacon.h"


class BeaconComponent : public ApplicationComponent
{
private:
	UdpBeacon* beacon = nullptr;
public:
	BeaconComponent(Application* app);
	void create();
	void destroy();
	void run();
};

