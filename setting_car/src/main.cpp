#include <iostream>
#include <string>
#include <pigpio.h>

#define SERVO_PIN 13  // GPIO13 (PWM1, пин 33)  сюда подключен сигнальный провод от сервопривода
#define ENGINE_PIN 12  // GPIO12 (PWM1, пин 32) сюда подключен сигнальный провод от двигателя


int servo = 1500;
int engine = 1500;

void update(){
    gpioServo(SERVO_PIN, servo);
    gpioServo(ENGINE_PIN, engine);

    std::cout << "engine=" << engine << "  servo=" << servo << std::endl;
}


// Пример обработчиков команд
void handleCommand(const std::string& cmd) {
    if (cmd == "hello") {
        std::cout << "Привет!\n";
    } else if (cmd == "time") {
        system("date");  // Выводит текущее время (использует системную команду)
    } else if (cmd == "S+") {
        servo+=50; update();
    } else if (cmd == "S-") {
        servo-=50; update();
    } else if (cmd == "E+") {
        engine+=50; update();

    } else if (cmd == "E-") {
        engine-=50; update();
    } else {
        std::cout << "Неизвестная команда: " << cmd << "\n";
    }
}


int main() {
    std::string input;

    gpioInitialise();

    std::cout << "Введите команду (q для выхода):\n";
    std::cout << "Для изменения частоты двигателя введите E+[ENTER] или E-[ENTER]\n";
    std::cout << "Для изменения частоты сервопривода введите S+[ENTER] или S-[ENTER]\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q") {
            std::cout << "Выход из программы.\n";
            break;
        }

        handleCommand(input);
    }

    gpioServo(SERVO_PIN, 0);  // Выключить сигнал
    gpioServo(ENGINE_PIN, 0);  // Выключить сигнал
    gpioTerminate();

    return 0;
}
