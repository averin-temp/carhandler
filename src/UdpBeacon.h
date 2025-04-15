#pragma once

#include <thread>
#include <atomic>
#include <iostream>

class UdpBeacon 
{
public:
    UdpBeacon();
    ~UdpBeacon();

    void start();
    void stop();
    void setPort(int);
    std::string message;
private:
    std::atomic<bool> running;
    std::atomic<bool> stop_thread_flag;
    std::atomic<bool> thread_running_flag;
    int sockfd;
    int port;

    std::thread* beaconThread;

    void beaconThreadWorker();
    void sendBroadcast();
};