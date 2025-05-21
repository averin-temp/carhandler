cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(car_setup)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 11)

add_executable( car_setup
    src/main.cpp
)

find_library(PIGPIO_LIB pigpio REQUIRED)
target_link_libraries(CarHandler ${PIGPIO_LIB})