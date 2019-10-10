cmake_minimum_required(VERSION 3.10.2)
project(roman C)

message(${CMAKE_BINARY_DIR})
set(CMAKE_C_STANDARD 99)

add_executable(roman main.c)


install(TARGETS roman DESTINATION ${CMAKE_BINARY_DIR}/../bin CONFIGURATIONS Debug)
