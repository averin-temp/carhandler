#pragma once
#include "Application.h"

class Application;

class ApplicationComponent
{
private:
	Application* application;
public:
	ApplicationComponent(Application* app);
	virtual void create();
	virtual void destroy();
	virtual void run();
};

