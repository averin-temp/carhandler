#include "DummyCarDriver.h"
#include <iostream>


#define SERVO_PIN 13  // GPIO13 (PWM1, пин 33)  сюда подключен сигнальный провод от сервопривода
#define ENGINE_PIN 12  // GPIO12 (PWM1, пин 32) сюда подключен сигнальный провод от двигателя

#define ENGINE_FORWARD 1575  // значение сигнала вперед (мск)
#define ENGINE_STOP 1500  // значение сигнала стоп (мск)
#define ENGINE_BACKWARD 1279  // значение сигнала назад (мск)

#define SERVO_LEFT 1560  // значение сигнала влево (мск)  (-70°)
#define SERVO_RIGHT 800  // значение сигнала вправо (мск) (70°)
#define SERVO_NONE 1200  // значение сигнала прямо (0°)


DummyCarDriver::DummyCarDriver()
{
    std::cout << "car dummy driver loading ..." << std::endl;
}

DummyCarDriver::~DummyCarDriver()
{
    std::cout << "~DummyCarDriver ..." << std::endl;
}


void DummyCarDriver::commandForward()
{
    std::cout << "\033[31mcommandBackward ...\033[0m" << std::endl;
}


void DummyCarDriver::commandBackward()
{
    std::cout << "\033[31mcommandBackward ...\033[0m" << std::endl;
}


void DummyCarDriver::commandStop()
{
    std::cout << "\033[31mcommandStop ...\033[0m" << std::endl;
}



void DummyCarDriver::commandTurnLeft()
{
    std::cout << "\033[31mcommandTurnLeft ...\033[0m" << std::endl;
}

void DummyCarDriver::commandTurnRight()
{
    std::cout << "\033[31m~commandTurnRight ...\033[0m" << std::endl;
}

void DummyCarDriver::commandTurnCenter()
{
    std::cout << "\033[31m~commandTurnCenter ...\033[0m" << std::endl;
}

void DummyCarDriver::calibrate()
{
    std::cout << "\033[31mCalibrate ...\033[0m" << std::endl;
}


