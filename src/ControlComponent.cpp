#include "ControlComponent.h"
#include "Command.h"
#include "constants.h"

#include <iostream>


Command* getCommand(UdpSocket* udp)
{
    std::vector<uint8_t> data = udp->getData();

    if (data.size() < 4) {
        // данных мало, не можем сформировать команду
        return nullptr;
    }

    Command* cmd = new Command;
    cmd->leftButton  = static_cast<char>(data[0]);
    cmd->rightButton = static_cast<char>(data[1]);
    cmd->upButton    = static_cast<char>(data[2]);
    cmd->downButton  = static_cast<char>(data[3]);

    // Можно добавить проверку, что значения 0 или 1:
    if ((cmd->leftButton != 0 && cmd->leftButton != 1) ||
        (cmd->rightButton != 0 && cmd->rightButton != 1) ||
        (cmd->upButton != 0 && cmd->upButton != 1) ||
        (cmd->downButton != 0 && cmd->downButton != 1)) 
    {
        delete cmd;
        return nullptr;
    }

    return cmd;
}

ControlComponent::ControlComponent(Application* app) : ApplicationComponent(app) {
	controlActive = false;
}

void ControlComponent::create()
{
	std::cout << "ControlComponent::create()\n";
	socket = new UdpSocket(NET_PORT_CAR_UDP_COMMAND_LISTENER);
}

void ControlComponent::destroy()
{
	std::cout << "ControlComponent::destroy()\n";
	delete socket;
}

void ControlComponent::run()
{

	if(application->controlEnable == true)
	{
		if(controlActive == true)
		{
			Command* command = getCommand(socket);
			if(command != nullptr) {
				controller.setState(command);
				delete command;
			}
		} else {
			// активация
			controlActive = true;
			controller.reset();
			std::cout << "ControlComponent activated.." << std::endl;
		}
		
	} 
	
	if(application->controlEnable == false)
	{
		if(controlActive == true)
		{
			// деактивация
			controlActive = false;
			controller.reset();
			std::cout << "ControlComponent deactivated.." << std::endl;
			
		} else {
			// ничего не происходит
		}
	}
}
