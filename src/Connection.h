#pragma once

#include <string>
#include <atomic>
#include <thread>

class Connection
{
    private:
        int socket;
        std::atomic<bool> worker_running;
        std::atomic<bool> waiting_command;
        std::atomic<bool> worker_thread_running;
        std::thread thread;
        std::atomic<bool> connection_active;
        std::string command;
     
        void startWorker();
    public:
        Connection(int sock_id);
        ~Connection();
        bool isActive();
        void disconnect();
        std::string getCommand();


        void startWaitingCommand();


        bool hasCommand();


};