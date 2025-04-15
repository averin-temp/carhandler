#include "Connection.h"
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <chrono>


Connection::Connection(int sock_id)
{
    socket = sock_id;
    connection_active = true;
    waiting_command = true;
    startWorker();
}

Connection::~Connection()
{
    worker_running = false;
    waiting_command = false;
    if(socket != -1)  { 
        close(socket);
        socket = -1;
    }


    if(thread.joinable()) {
        thread.join();
    }
}





bool Connection::isActive()
{
    return connection_active;
}


void Connection::disconnect()
{
    close(socket);
}

void Connection::startWaitingCommand()
{
    if(waiting_command == false)
    {
        if(worker_thread_running && worker_running && connection_active) 
            waiting_command = true;
    }
}

bool Connection::hasCommand()
{
    return (waiting_command == false && command != "");
}

std::string Connection::getCommand()
{
    std::string c = command;
    command = "";
    return c;
}


void Connection::startWorker()
{
    worker_running = true;
    thread = std::thread([this](){

        worker_thread_running = true;


        char buffer[1024];
        int bytes_received;

        while(worker_running)
        {
            if(waiting_command)
            {
                bytes_received = recv(socket, buffer, sizeof(buffer), 0);
                if(bytes_received == 0) {
                    connection_active = false;
                    close(socket);
                    socket = -1;
                    worker_running = false;
                } else if(bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    command = std::string(buffer);
                    waiting_command = false;
                    std::cout << "Received command: " << command << std::endl;
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }   
        }

        worker_thread_running = false;
    });
}