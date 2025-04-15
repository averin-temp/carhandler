#include "BeaconComponent.h"


#include <iostream>

BeaconComponent::BeaconComponent(Application* app, IMainComponent* pMain) : ApplicationComponent(app) {
	pMainComponents = pMain;
}

void BeaconComponent::create()
{
	beacon = new UdpBeacon();
	beacon->start();
	std::cout << "BeaconComponent::create()\n";
}

void BeaconComponent::destroy()
{
	delete beacon;
	std::cout << "BeaconComponent::destroy()\n";
}

void BeaconComponent::run()
{
	AppState s = pMainComponents->getState();

	if(s == AppState::STATE_WAITING)
	{
		beacon->message = "WAITING";
	}

	if(s == AppState::STATE_CONNECTED)
	{
		beacon->message = "CONNECTED";
	}
}