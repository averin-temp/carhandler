cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(CarHandler)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 11)

add_executable( CarHandler
    src/Application.h
    src/ApplicationComponent.h
    src/AppState.h
    src/BeaconComponent.h
    src/Command.h
    src/Connection.h
    src/ConnectionListener.h
    src/constants.h
    src/ControlComponent.h
    src/MainComponent.h
    src/UdpBeacon.h
    src/UdpSocket.h
    src/CarController.h
    src/DummyCarDriver.h
    src/ICarDriver.h

    src/Application.cpp
    src/ApplicationComponent.cpp
    src/BeaconComponent.cpp
    src/Connection.cpp
    src/ConnectionListener.cpp
    src/ControlComponent.cpp
    src/MainComponent.cpp
    src/UdpBeacon.cpp
    src/CarController.cpp
    src/CarDriver.cpp
    src/main.cpp
)
