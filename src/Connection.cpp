#include "Connection.h"
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <cstring>



unsigned long time_ms() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


Connection::Connection(int sock_id)
{
    socket = sock_id;
    connection_active = true;
    // Установка таймаута
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "setsockopt failed" << std::endl;
    }

    last_sended = time_ms();
    last_received = last_sended;
    keep_alive_interval = 5000;
    keep_alive_max_interval = 10000;

    startWorker();
}

Connection::~Connection()
{
    worker_running = false;
    waiting_command = false;
    if(socket != -1)  { 
        close(socket);
        socket = -1;
    }
    if(thread.joinable()) {
        thread.join();
    }

    std::cout << "Connection:  destructed " << std::endl;
}





bool Connection::isActive()
{
    return connection_active;
}


bool Connection::hasCommand()
{
    return false;
}

std::string Connection::getCommand()
{
    return "";
}



std::vector<char>& Connection::getBuffer() 
{
    return inputBuffer;
}



void Connection::writeString(const std::string& data) {
    ssize_t total_written = 0;
    ssize_t to_write = data.size();
    const char* buffer = data.c_str();

    while (total_written < to_write) {
        ssize_t written = write(socket, buffer + total_written, to_write - total_written);
        if (written == -1) {
            if (errno == EINTR)
                continue; // прерван вызов - повторить
            throw std::runtime_error(std::string("Write failed: ") + std::strerror(errno));
        }
        if (written == 0) {
            throw std::runtime_error("Connection closed while writing");
        }
        total_written += written;
    }
}

std::vector<char> Connection::read()
{
    char buffer[1024];
    
    int bytes_received = recv(socket, buffer, sizeof(buffer), 0);
                
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Timeout
            std::cout << "Connection:  recv Timeout " << std::endl;
        }
        return {};
    } 
    
    if(bytes_received > 0) {
        return std::vector<char>(buffer, buffer + bytes_received);
    }

    //bytes_received == 0
    throw std::runtime_error("Connection closed by peer.");
}

std::string Connection::extractNextCommand(std::vector<char>& buffer) {
    // Преобразуем буфер в строку
    std::string data(buffer.begin(), buffer.end());

    size_t pos = data.find('|');
    if (pos == std::string::npos) {
        // Нет полного разделителя — команда не завершена
        return "";
    }

    std::string command = data.substr(0, pos);

    // Удаляем использованные символы из буфера
    buffer.erase(buffer.begin(), buffer.begin() + pos + 1);

    return command;
}


void Connection::sendCommand(std::string command)
{
    writeString(command + "|");
}

std::string Connection::readCommand()
{

    std::string command = extractNextCommand(inputBuffer);

    if(command == "")
    {
        std::vector<char> data = read();
        if(data.size() > 0) {
            inputBuffer.insert(inputBuffer.end(), data.begin(), data.end());
            command = extractNextCommand(inputBuffer);
        }
    }

    return command;
}


void Connection::startWorker()
{
    std::cout << "Connection::startWorker()" << std::endl;

    worker_running = true;
    thread = std::thread([this](){

        std::cout << "Connection:  worker started" << std::endl;
        
        while(worker_running) {
            try {
                std::string command = readCommand();
                std::cout << "Command received:  (" << command << ")" << std::endl;

                if(command != "") {
                    last_received = time_ms();
                    //  тут пока пусто, здесь команда куда-то кладется, чтобы извне ее получить
                }

                // Отправка сигнала о наличии подключения
                if((time_ms() - last_sended) > keep_alive_interval)
                {
                    sendCommand("KEEP_ALIVE");
                    last_sended = time_ms();
                    std::cout << "send KEEP_ALIVE" << std::endl;
                }

                // проверка , не истекло ли время молчания
                if((time_ms() - last_received) > keep_alive_max_interval) {
                    throw std::runtime_error("Disconnected: keep_alive_max_interval reached");
                }

            } catch (const std::exception& e) {
                std::cout << "Exception Error: " << e.what() << std::endl;
                connection_active = false;
                worker_running = false;
            }                    
        }

        std::cout << "Connection:  worker ended" << std::endl;
    });
}


