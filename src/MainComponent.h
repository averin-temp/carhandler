#pragma once

#include "ApplicationComponent.h"
#include "ConnectionListener.h"
#include "Connection.h"
#include "AppState.h"

class MainComponent : public ApplicationComponent
{
	enum class State{
		STATE_WAITING_CONNECTION = 1,
		STATE_CONNECTED = 2
	};

	State state;
	Connection* pConnection = nullptr;
	ConnectionListener listener;

	void processCommand(std::string);
public:
	MainComponent(Application* pApp);
	~MainComponent();
	void create();
	void destroy();
	void run();
	AppState getState();
};

