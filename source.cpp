#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <locale>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <pigpio.h>
#include <cstring>
#include <csignal>



bool isTest = true;      // �� ��������� �������� �����
std::string serverIp = "127.0.0.1";  // �� ��������� localhost
int serverPort = 50500;     // �� ��������� ���� 50500
int localPort = 50501;      // �� ��������� ���� 50501




class CarHandler
{
protected:
    bool initialized = false;

    enum EngineState {
        FULL_FORWARD = 1,
        FULL_BACKWARD = 2,
        STOP = 3
    };

    EngineState engineState = EngineState::STOP;

public:
    CarHandler();
    virtual ~CarHandler();
    virtual void commandForward();
    virtual void commandBackward();
    virtual void commandStop();
    virtual void commandTurnLeft();
    virtual void commandTurnRight();
    virtual void commandTurnCenter();
    virtual void processCommand(const std::string& command);
};



#define BUFFER_SIZE 1024

class Connection
{
private:
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    bool connected;
    std::string serverIp;
    int serverPort;

public:
    Connection(const std::string& serverIp, int serverPort);
    ~Connection();
    bool connect();
    bool exists();
    std::string getCommand();
};

#define SERVO_PIN 13  // GPIO13 (PWM1, ��� 33)  ���� ��������� ���������� ������ �� ������������
#define ENGINE_PIN 12  // GPIO12 (PWM1, ��� 32) ���� ��������� ���������� ������ �� ���������

#define SIGNAL_ENGINE_FULL_FORWARD 1575  // �������� ������� ������ (���)
#define SIGNAL_ENGINE_STOP 1500  // �������� ������� ���� (���)
#define SIGNAL_ENGINE_FULL_BACKWARD 1279  // �������� ������� ����� (���)

#define SIGNAL_ROTATE_FULLLEFT 1560  // �������� ������� ������ ����� (���)
#define SIGNAL_ROTATE_FULLRIGHT 800  // �������� ������� ������ ������ (���)
#define SIGNAL_ROTATE_NONE 1200  // �������� ������� ������� 0


CarHandler::CarHandler()
{
    std::cout << "����������� ������� �������" << std::endl;
    if (gpioInitialise() < 0) {
        throw std::runtime_error("gpioInitialise: �� ������� ���������������� ����������� � pigpio!");
    }
}

CarHandler::~CarHandler()
{
    gpioServo(SERVO_PIN, 0);  // ��������� ������
    gpioServo(ENGINE_PIN, 0);  // ��������� ������
    gpioTerminate();
}


void CarHandler::commandForward()
{
    gpioServo(ENGINE_PIN, SIGNAL_ENGINE_FULL_FORWARD);
    engineState = EngineState::FULL_FORWARD;
}


void CarHandler::commandBackward()
{
    gpioServo(ENGINE_PIN, SIGNAL_ENGINE_FULL_BACKWARD);
    gpioDelay(200000);
    gpioServo(ENGINE_PIN, SIGNAL_ENGINE_STOP);
    gpioDelay(200000);
    gpioServo(ENGINE_PIN, SIGNAL_ENGINE_FULL_BACKWARD);
    engineState = EngineState::FULL_BACKWARD;
}


void CarHandler::commandStop()
{
    gpioServo(ENGINE_PIN, SIGNAL_ENGINE_STOP);
    engineState = EngineState::STOP;
}



void CarHandler::commandTurnLeft()
{
    gpioServo(SERVO_PIN, SIGNAL_ROTATE_FULLLEFT);
}

void CarHandler::commandTurnRight()
{
    gpioServo(SERVO_PIN, SIGNAL_ROTATE_FULLRIGHT);
}


void CarHandler::commandTurnCenter()
{
    gpioServo(SERVO_PIN, SIGNAL_ROTATE_NONE);
}


void CarHandler::processCommand(const std::string& command)
{
    if (command == "FORWARD") { commandForward(); return; }
    if (command == "BACKWARD") { commandBackward(); return; }
    if (command == "STOP") { commandStop(); return; }
    if (command == "TURNLEFT") { commandTurnLeft(); return; }
    if (command == "TURNRIGHT") { commandTurnRight(); return; }
    if (command == "TURNCENTER") { commandTurnCenter(); return; }
}




// ������� ��� ������� ���������� ��������� ������
std::map<std::string, std::string> parseArguments(int argc, char* argv[]) {
    std::map<std::string, std::string> params;

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) {
            throw std::invalid_argument("������������ ���������� ��� ���������: " + std::string(argv[i]));
        }
        std::string key = argv[i];
        std::string value = argv[i + 1];
        params[key] = value;
    }

    return params;
}



// ����������� ������ ��� ����������� ����������� �������� �����
void setLocale()
{
    try {
        std::locale::global(std::locale("")); // ������ �� ��������� �������
        std::wcout.imbue(std::locale());
        std::wcin.imbue(std::locale());
    }
    catch (const std::exception& e) {
        std::cerr << "������ ��� ��������� ������: " << e.what() << std::endl;
    }
}




Connection::Connection(const std::string& serverIp, int serverPort)
{
    this->serverPort = serverPort;
    this->serverIp = serverIp;
    this->connected = false;
}


Connection::~Connection()
{
    if (connected) {
        close(clientSocket);
    }
}


bool Connection::exists()
{
    return connected;
}


std::string Connection::getCommand()
{
    if (connected)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "���������� ������� ��������" << std::endl;
            close(clientSocket);
            connected = false;
            return "";
        }

        buffer[bytesReceived] = '\0';
        std::cout << "������� ��������: " << buffer << std::endl;
        return buffer;
    }

    return "";
}



// ������� ��� ����������� � �������
int connectToServer(const std::string& serverIp, int serverPort) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "������ �������� ������: " << strerror(errno) << std::endl;
        return -1;
    }

    // ��������� ������ �������
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIp.c_str());

    // ������� �����������
    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "������ ����������� � �������: " << strerror(errno) << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "����������� � ������� ������� �����������!" << std::endl;
    return sock;
}


bool Connection::connect()
{
    if (!connected)
    {
        std::cout << "������� ����������� � ������� " << serverIp << ":" << serverPort << "..." << std::endl;

        clientSocket = connectToServer(serverIp, serverPort);
        if (clientSocket != -1) {
            connected = true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

Connection* connection = nullptr;
CarHandler* carHandler = nullptr;

Connection* getConnection(const std::string& ip, int port)
{
    return new Connection(ip, port);
}


bool running = true;

void signalHandler(int signum) {
    std::cout << "\n������ ������: " << signum << " (Ctrl+C)" << std::endl;
    running = false;
}




int main(int argc, char* argv[])
{

    //signal(SIGINT, signalHandler); // ��������� ���������� ��� SIGINT

    try {

        // ��������� ��������� ��������� ������
        auto params = parseArguments(argc, argv);

        // ��������� �������� ����������, ���� ��� ��������
        if (params.find("-server_ip") != params.end()) {
            serverIp = params["-server_ip"];
        }
        if (params.find("-server_port") != params.end()) {
            serverPort = std::stoi(params["-server_port"]);
        }
        if (params.find("-local_port") != params.end()) {
            localPort = std::stoi(params["-local_port"]);
        }
        if (params.find("-test") != params.end()) {
            isTest = params["-test"] == "1";
        }

        std::cout << "���������: server_ip = " << serverIp
            << ", server_port = " << serverPort
            << ", local_port = " << localPort
            << ", test = " << isTest
            << std::endl;


        // ������� ������ �������
        carHandler = new CarHandler();

        connection = getConnection(serverIp, serverPort);

        while (running)
        {
            if (!connection->exists())
            {
                connection->connect();
            }
            else {
                std::string command = connection->getCommand();
                if (command == "FORWARD") carHandler->commandForward();
                if (command == "BACKWARD") carHandler->commandBackward();
                if (command == "STOP") carHandler->commandStop();
                if (command == "LEFT") carHandler->commandTurnLeft();
                if (command == "CENTER") carHandler->commandTurnCenter();
                if (command == "RIGHT") carHandler->commandTurnRight();
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
        return 1;
    }

    if (connection) delete connection;
    if (carHandler) delete carHandler;

    return 0;
}
