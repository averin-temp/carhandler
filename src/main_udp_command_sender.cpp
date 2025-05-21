#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include "Command.h"
#include "constants.h"

ssize_t sendCommand(int sockfd, const struct sockaddr_in& addr, const Command& cmd) {
    return sendto(sockfd, &cmd, sizeof(cmd), 0,
                  reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(NET_PORT_CAR_UDP_COMMAND_LISTENER);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    for (int i = 0; i < 10; ++i) {
        Command command{};
        // Для примера: просто меняем значения кнопок циклично
        command.leftButton = (i % 2 == 0) ? 1 : 0;
        command.rightButton = (i % 3 == 0) ? 1 : 0;
        command.upButton = (i % 4 == 0) ? 1 : 0;
        command.downButton = (i % 5 == 0) ? 1 : 0;

        ssize_t sent = sendCommand(sockfd, addr, command);
        if (sent < 0) {
            perror("sendto");
        } else {
            std::cout << "Sent command " << i + 1 << " (" << sent << " bytes)\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    close(sockfd);
    return 0;
}
