#pragma once

#include <vector>
#include "ApplicationComponent.h"

class ApplicationComponent;

class Application
{
private:
	bool running;
	std::vector<ApplicationComponent*> components;
	Application* application;
public:
	Application();
	~Application();
	void addComponent(ApplicationComponent* pComponent);
	void run();
	void stop();
	void createComponents();
	void runComponents();
	void destroyComponents();
};

