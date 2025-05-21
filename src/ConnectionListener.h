#pragma once

#include <thread>

class ConnectionListener
{
private:
    int sock;
    int port;
    int connection;
    bool listening;
    std::thread thread;
    void startWorker();
public:
    ConnectionListener();
    ~ConnectionListener();
    void start();
    void stop();
    bool isListening();
    int getConnectionSocket();
    bool hasConnection();
};