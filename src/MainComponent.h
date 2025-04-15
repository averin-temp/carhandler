#pragma once
#include "ApplicationComponent.h"
#include "TcpServer.h"
#include "Connection.h"
#include "IStreamComponent.h"
#include "IMainComponent.h"
#include "AppState.h"

class MainComponent : public ApplicationComponent, public IMainComponent
{

	AppState state = AppState::STATE_WAITING;
	Connection* pConnection = nullptr;
	TcpServer server;

	void processCommand(std::string);
	IStreamComponent* pStreamer;
public:
	MainComponent(Application* pApp, IStreamComponent* streamer);
	void create();
	void destroy();
	void run();
	AppState getState();

};

