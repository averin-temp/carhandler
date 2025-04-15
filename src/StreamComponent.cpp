#include "StreamComponent.h"
#include <iostream>
#include <string>
#include "constants.h"


enum class State {
    STATE_RUNNING,
    STATE_STOPPED
};

State state = State::STATE_STOPPED;


std::string getCommandString()
{
    // Запускаем ffmpeg через system() и сразу ставим в фоновый режим с &
    return "ffmpeg -f v4l2 -i /dev/video10 -s 640x280 -b:v 1M -f rtsp -rtsp_transport udp -preset ultrafast -tune zerolatency -flush_packets 1 rtsp://localhost:8554/mystream";
}



StreamComponent::StreamComponent(Application* app) : ApplicationComponent(app) {
	
}

void StreamComponent::create()
{
    startStream();
}

void StreamComponent::destroy()
{
	stopStream();
}

void StreamComponent::run()
{

}


void StreamComponent::startStream()
{
    if(state == State::STATE_STOPPED)
    {
        std::string command = getCommandString();
        
        int result = std::system(command.c_str());
        if (result == 0) {
            std::cout << "stream started" << std::endl;
            state = State::STATE_RUNNING;
        } else {
            std::cout << "Failed to start ffmpeg stream" << std::endl;
        }
    }
}


void StreamComponent::stopStream()
{
    if(state == State::STATE_RUNNING)
    {
        // Завершаем процесс ffmpeg по имени через pkill
        std::string killCommand = "pkill ffmpeg";
        std::system(killCommand.c_str());
        state = State::STATE_STOPPED;

        std::cout << "Stream stopped" << std::endl;
    }
}