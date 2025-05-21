#include "Application.h"

#include <iostream>

Application::Application()
{
	running = false;

	bool controlEnable = false;
	bool beaconActive = false;
	bool connectionEstablished = false;

	std::cout << "Application::Application()" << std::endl;
}

Application::~Application()
{
	std::cout << "Application::~Application()" << std::endl;
}


void Application::stop()
{
	running = false;
}


void Application::run()
{
	if (running) return;
	running = true;

	try
	{
		try
		{
			createComponents();
			runComponents();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		destroyComponents();
		
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	running = false;

	std::cout << "Application stopped" << std::endl;
}


void Application::createComponents() 
{
	for (ApplicationComponent* component : components)
	{
		component->create();
	}
}

void Application::runComponents() 
{
	while (running)
	{
		for (ApplicationComponent* component : components)
		{
			component->run();
		}
	}
}
void Application::destroyComponents()
{
	for (ApplicationComponent* component : components)
	{
		component->destroy();
	}
}

void Application::addComponent(ApplicationComponent* pComponent)
{
	components.push_back(pComponent);
}