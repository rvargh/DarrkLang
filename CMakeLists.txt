cmake_minimum_required(VERSION 3.15)
project(DarkkLang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_include_directories(${PROJECT_NAME} PRIVATE include)
add_executable(${PROJECT_NAME} src/main.cpp)