#pragma once


#include <atomic>
#include <thread>
#include "Command.h"
#include "Connection.h"

class TcpServer
{

private:

    enum class State{
        STOPPED = 0,
        LISTENING = 1,
        CONNECTED = 2
    };

    State state = State::STOPPED;
    Connection* pConnection = nullptr;
    bool listening = false;
    std::atomic<bool> thread_running;
    std::thread thread;
    void startThread();
    void stopThread();
    int sock;
public:
    TcpServer();
    ~TcpServer();
    void startListen();
    Connection* getConnection();
    void stop();
    bool isStopped();
    bool isConnected();
    bool isListening();
    Command* getCommand();
};
