# Project definition
cmake_minimum_required(VERSION 3.1)
project(Hack)

set(SOURCES main.cpp memory_manager.cpp memory_manager.h csgo.h entity.cpp entity.h vector3.cpp vector3.h local_player.cpp local_player.h)
add_executable(${PROJECT_NAME} ${SOURCES})
set (CMAKE_C_FLAGS "-m32")
