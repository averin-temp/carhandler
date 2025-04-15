#include "MainComponent.h"
#include <iostream>
#include <vector>


MainComponent::MainComponent(Application* app, IStreamComponent* streamer) : ApplicationComponent(app) {
	pStreamer = streamer;
}


void MainComponent::create()
{
	std::cout << "MainComponent::create()\n";
}

void MainComponent::destroy()
{

	server.stop();
	std::cout << "MainComponent::destroy()\n";
}

AppState MainComponent::getState()
{
	return state;
}



void MainComponent::run()
{
	if(state == AppState::STATE_WAITING)
	{
		if(server.isStopped())
		{
			server.startListen();
		}
		
		if(server.isConnected())
		{
			pConnection = server.getConnection();
			state = AppState::STATE_CONNECTED;
		}
	}


	if(state == AppState::STATE_CONNECTED)
	{
		if(pConnection->isActive())
		{
			if(pConnection->hasCommand())
			{
				std::string command = pConnection->getCommand();

				processCommand(command);





				pConnection->startWaitingCommand();
			}
		}
		else
		{
			delete pConnection;
			pConnection = nullptr;
			pStreamer->stopStream();
			state = AppState::STATE_WAITING;
		}
		
	}
}


std::vector<std::string> splitCommand(const std::string& command) {
    std::vector<std::string> result;
    size_t start = 0;
    while (start < command.size()) {
        size_t end = command.find(' ', start);
        if (end == std::string::npos) end = command.size();
        if (end > start) {
            result.push_back(command.substr(start, end - start));
        }
        start = end + 1;
    }
    return result;
}



void MainComponent::processCommand(std::string command)
{

	std::vector<std::string> parts = splitCommand(command);

	if(parts[0] == "TARGET_DEVICE")
	{
		std::string ip = parts[1];
		std::string port = parts[2];

		pStreamer->startStream();
	}

	if(parts[0] == "DISCONNECT_CAR")
	{
		pStreamer->stopStream();
	}
}


