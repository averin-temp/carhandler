cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(MyProject)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cpp")

add_executable(MyProject ${SOURCES})

# Указать файлы для компиляции
#add_executable(MyProject src/main.cpp)

