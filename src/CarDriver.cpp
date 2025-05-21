#include "CarDriver.h"
#include <pigpio.h>
#include <thread>
#include <iostream>


#define SERVO_PIN 13  // GPIO13 (PWM1, пин 33)  сюда подключен сигнальный провод от сервопривода
#define ENGINE_PIN 12  // GPIO12 (PWM1, пин 32) сюда подключен сигнальный провод от двигателя

#define ENGINE_FORWARD 1590 // значение сигнала вперед (мск)
#define ENGINE_STOP 1500  // значение сигнала стоп (мск)
#define ENGINE_BACKWARD 1390  // значение сигнала назад (мск)

#define SERVO_LEFT 1950  // значение сигнала влево (мск)  (-70°)
#define SERVO_NONE 1600  // значение сигнала прямо (0°)
#define SERVO_RIGHT 1250  // значение сигнала вправо (мск) (70°)


CarDriver::CarDriver()
{
    std::cout << "car driver loading ..." << std::endl;
    if (gpioInitialise() < 0) {
        throw std::runtime_error("gpioInitialise: can't establish connection to pigpio!");
    }
}

CarDriver::~CarDriver()
{
    // все выключаем
    gpioServo(SERVO_PIN, 0);  // Выключить сигнал
    gpioServo(ENGINE_PIN, 0);  // Выключить сигнал
    gpioTerminate();
}


void CarDriver::commandForward()
{
    gpioServo(ENGINE_PIN, ENGINE_FORWARD);
    std::cout << "\033[31mFORWARD ...\033[0m" << std::endl;
}


void CarDriver::commandBackward()
{
    gpioServo(ENGINE_PIN, ENGINE_BACKWARD);
    std::cout << "\033[31mBACK ...\033[0m" << std::endl;
}


void CarDriver::commandStop()
{
    gpioServo(ENGINE_PIN, ENGINE_STOP);
    std::cout << "\033[31mSTOP ...\033[0m" << std::endl;
}



void CarDriver::commandTurnLeft()
{
    gpioServo(SERVO_PIN, SERVO_LEFT);
    std::cout << "\033[31mLEFT ...\033[0m" << std::endl;
}

void CarDriver::commandTurnRight()
{
    gpioServo(SERVO_PIN, SERVO_RIGHT);
    std::cout << "\033[31mRIGHT ...\033[0m" << std::endl;
}


void CarDriver::commandTurnCenter()
{
    gpioServo(SERVO_PIN, SERVO_NONE);
    std::cout << "\033[31mCENTER ...\033[0m" << std::endl;
}


void CarDriver::calibrate()
{
    gpioServo(SERVO_PIN, 1500);
    gpioServo(ENGINE_PIN, 1500);
    gpioDelay(3000000);
    gpioServo(SERVO_PIN, 0);
    gpioServo(ENGINE_PIN, 0);
}