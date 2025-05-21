#include "CarDriver.h"
#include <pigpio.h>
#include <iostream>


#define SERVO_PIN 13  // GPIO13 (PWM1, пин 33)  сюда подключен сигнальный провод от сервопривода
#define ENGINE_PIN 12  // GPIO12 (PWM1, пин 32) сюда подключен сигнальный провод от двигателя

#define ENGINE_FORWARD 1575  // значение сигнала вперед (мск)
#define ENGINE_STOP 1500  // значение сигнала стоп (мск)
#define ENGINE_BACKWARD 1279  // значение сигнала назад (мск)

#define SERVO_LEFT 1560  // значение сигнала влево (мск)  (-70°)
#define SERVO_RIGHT 800  // значение сигнала вправо (мск) (70°)
#define SERVO_NONE 1200  // значение сигнала прямо (0°)


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
}


void CarDriver::commandBackward()
{
    gpioServo(ENGINE_PIN, ENGINE_BACKWARD);
}


void CarDriver::commandStop()
{
    gpioServo(ENGINE_PIN, ENGINE_STOP);
}



void CarDriver::commandTurnLeft()
{
    gpioServo(SERVO_PIN, SERVO_LEFT);
}

void CarDriver::commandTurnRight()
{
    gpioServo(SERVO_PIN, SERVO_RIGHT);
}


void CarDriver::commandTurnCenter()
{
    gpioServo(SERVO_PIN, SERVO_NONE);
}


