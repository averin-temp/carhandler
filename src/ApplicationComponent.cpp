#include "ApplicationComponent.h"

#include <iostream>


ApplicationComponent::ApplicationComponent(Application* app)
{
	application = app;
}


void ApplicationComponent::create()
{
	std::cout << "ApplicationComponent::create\n";
}

void ApplicationComponent::destroy()
{
	std::cout << "ApplicationComponent::destroy()\n";
}

void ApplicationComponent::run()
{
}

