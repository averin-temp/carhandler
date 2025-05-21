#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <vector>

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
        std::vector<char> read();
        void writeString(const std::string& data);
        std::vector<char>& getBuffer();
        std::vector<char> inputBuffer;
        std::string extractNextCommand(std::vector<char>& buffer);
        void sendCommand(std::string);
        std::string readCommand();


        unsigned long last_sended;
        unsigned long last_received;
        unsigned long keep_alive_interval;
        unsigned long keep_alive_max_interval;


    public:
        Connection(int sock_id);
        ~Connection();
        bool isActive();
        void disconnect();
        std::string getCommand();


        void startWaitingCommand();


        bool hasCommand();





};