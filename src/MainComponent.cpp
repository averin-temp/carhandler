#include "MainComponent.h"
#include <iostream>
#include <vector>


MainComponent::MainComponent(Application* app) : ApplicationComponent(app) {
	
}

MainComponent::~MainComponent()
{
	
}

void MainComponent::create()
{
	std::cout << "MainComponent::create()\n";
	state = State::STATE_WAITING_CONNECTION;
}

void MainComponent::destroy()
{
	std::cout << "MainComponent::destroy()\n";

	if(pConnection){
		delete pConnection;
	}
	
	
}

void MainComponent::run()
{
	if(state == State::STATE_WAITING_CONNECTION) {
		if(listener.isListening() == false) {
			if(listener.hasConnection()) {
				pConnection = new Connection(listener.getConnectionSocket());
				state = State::STATE_CONNECTED;
			} else {
				listener.start();
			}
		}
	}

	if(state == State::STATE_CONNECTED) {
		if(pConnection->isActive()) {
			std::string command = pConnection->getCommand();
		} else {
			delete pConnection;
			pConnection = nullptr;
			state = State::STATE_WAITING_CONNECTION;
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


