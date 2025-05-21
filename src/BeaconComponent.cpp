#include "BeaconComponent.h"


#include <iostream>

BeaconComponent::BeaconComponent(Application* app) : ApplicationComponent(app) {

}

void BeaconComponent::create()
{
	beacon = new UdpBeacon();
	beacon->message = "WAITING";
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
	
}