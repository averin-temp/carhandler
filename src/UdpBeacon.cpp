#include "UdpBeacon.h"

#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "constants.h"


UdpBeacon::UdpBeacon()
{
    port = NET_PORT_CAR_BEACON;
    running = false;
    stop_thread_flag = false;
    thread_running_flag = false;
    message = "WAITING";
}

UdpBeacon::~UdpBeacon()
{
    stop();
	std::cout << "UdpBeacon::~UdpBeacon()" << std::endl;
}


void UdpBeacon::beaconThreadWorker()
{
    thread_running_flag = true;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("setsockopt");
        close(sock);
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");




    
    while (stop_thread_flag == false) 
    {

        const char* imessage = message.c_str();
        ssize_t sent = sendto(sock, imessage, strlen(imessage), 0, (sockaddr*)&addr, sizeof(addr));
        if (sent < 0) {
            perror("sendto");
        } else {
            std::cout << "Broadcast sent\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    close(sock);

    thread_running_flag = false;
}





void UdpBeacon::start()
{
    if(!running)
    {
        running = true;
        stop_thread_flag = false;
        beaconThread = new std::thread(&UdpBeacon::beaconThreadWorker, this);
    } 
}

void UdpBeacon::stop()
{
    if(running)
    {
        if(thread_running_flag)
        {
            stop_thread_flag = true;
            beaconThread->join();    
        }
        delete beaconThread;
        beaconThread = nullptr;
        running = false;
    } 
}

