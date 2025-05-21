#pragma once
#include "Application.h"

class Application;

class ApplicationComponent
{
protected:
	Application* application;
public:
	ApplicationComponent(Application* app);
	virtual void create();
	virtual void destroy();
	virtual void run();
};

