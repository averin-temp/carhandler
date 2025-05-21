#include "ConnectionListener.h"

#include <chrono>
#include <sys/socket.h> // socket
#include <arpa/inet.h> // sockaddr_in
#include <iostream> // runtime_error
#include <unistd.h>  // для close()

ConnectionListener::ConnectionListener()
{
    port = 50501;
    sock = -1;
    listening = false;
    connection = -1;
}

ConnectionListener::~ConnectionListener()
{
    stop();
}

int ConnectionListener::getConnectionSocket()
{
    int s = connection;
    connection = -1;
    return s;
}


bool ConnectionListener::hasConnection()
{
    return connection != -1;
}

void ConnectionListener::start()
{
    if(listening == false)
    {
        std::cout << "ConnectionListener: start()" << std::endl;


        stop();
        listening = true;

        std::cout << "ConnectionListener: started" << std::endl;

        startWorker();
    }
}

void ConnectionListener::stop()
{
    std::cout << "ConnectionListener: stop()" << std::endl;

    if (sock != -1) {
        shutdown(sock, SHUT_RDWR); 
        close(sock);
        sock = -1;
    }


    if(thread.joinable()) {
        thread.join();
    }
    if(connection != -1) {
        shutdown(connection, SHUT_RDWR); 
        close(connection);
    }
    listening = false;

    std::cout << "ConnectionListener: stopped" << std::endl;
}

bool ConnectionListener::isListening()
{
    return listening;
}

void ConnectionListener::startWorker()
{
    thread = std::thread([this]() {

        std::cout << "ConnectionListener: worker start!" << std::endl;

        try {
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock == -1) {
                throw std::runtime_error("Failed to create socket");
            }

            sockaddr_in addr{AF_INET, htons(port), INADDR_ANY};

            if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
                throw std::runtime_error("Failed to bind socket");
            }

            if (listen(sock, 1) == -1) {
                throw std::runtime_error("Failed to listen on socket");
            }

            std::cout << "Server started ..." << std::endl;

            connection = accept(sock, nullptr, nullptr);
            if (connection == -1) {
                throw std::runtime_error("Failed to accept connection");
            }

            std::cout << "client connected!" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Exception Error: " << e.what() << std::endl;
        }

        if(sock != -1) {
            close(sock);
            sock = -1;
        }

        std::cout << "ConnectionListener: worker stop!" << std::endl;

        listening = false;
    });
}