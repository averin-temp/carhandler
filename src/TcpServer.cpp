#include "TcpServer.h"
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "constants.h"

TcpServer::TcpServer()
{
    thread_running = false;
}


TcpServer::~TcpServer()
{
    stop();
}


Connection* TcpServer::getConnection()
{


    Connection* c = pConnection;
    pConnection = nullptr;

    stop();

    return c;
}


void TcpServer::startListen()
{

    stop();

    if(state == State::STOPPED)
    {
        state = State::LISTENING;

        thread = std::thread([this]() {
            thread_running = true;
    
            int port = COMMAND_TCP_PORT;
            int client;
    
            try {
    
                sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock == -1) 
                {
                    throw std::runtime_error("Failed to create socket");
                }
    
                sockaddr_in addr{AF_INET, htons(port), INADDR_ANY};
    
                if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1) 
                {
                    close(sock);
                    throw std::runtime_error("Failed to bind socket");
                }
    
                if (listen(sock, 1) == -1) {
                    close(sock);
                    throw std::runtime_error("Failed to listen on socket");
                }
    
                client = accept(sock, nullptr, nullptr);
                if (client == -1) {
                    close(sock);
                    throw std::runtime_error("Failed to accept connection");
                }
    
                pConnection = new Connection(client);

                state = State::CONNECTED;
                
                close(sock);
                sock = -1;
    
                std::cout << "Server started and client connected!" << std::endl;
            } catch (const std::exception& e) {
    
                if(sock != -1) {
                    close(sock);
                    sock = -1;
                }

                state = State::STOPPED;
    
                std::cout << "Error: " << e.what() << std::endl;
            }
    
            thread_running = false;
        });

    }
}


void TcpServer::stop()
{
    if(thread_running)
    {
        if(pConnection != nullptr)
            delete pConnection;

        if(sock != -1) {
            close(sock);
        }
        if (thread.joinable()) {
            thread.join();
        }

        state = State::STOPPED;
    }
}

bool TcpServer::isConnected()
{
    return state == State::CONNECTED;
}


bool TcpServer::isListening()
{
    return state == State::LISTENING;
}


bool TcpServer::isStopped()
{
    return state == State::STOPPED;
}


Command* TcpServer::getCommand()
{
    return nullptr;
}

