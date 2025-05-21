#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <cstring>
#include <errno.h>

class UdpSocket {
public:
    UdpSocket(int port) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket creation failed");
            throw std::runtime_error("Failed to create socket");
        }

        // Сделать неблокирующим
        int flags = fcntl(sockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind failed");
            close(sockfd);
            throw std::runtime_error("Failed to bind socket");
        }
    }

    ~UdpSocket() {
        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    std::vector<uint8_t> getData() {
        std::vector<uint8_t> buffer(2048);
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);

        ssize_t len = recvfrom(sockfd, buffer.data(), buffer.size(), 0,
                               (sockaddr*)&clientAddr, &addrLen);

        if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return {}; // Нет данных, не блокируемся
            } else {
                perror("recvfrom failed");
                return {};
            }
        }

        buffer.resize(len);
        return buffer;
    }

private:
    int sockfd;
};
