cmake_minimum_required(VERSION 3.14)
project(2view LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets Gui REQUIRED)

add_executable(2view main.cpp canvas.cpp view.cpp draw.cpp)

target_link_libraries(2view PRIVATE Qt6::Widgets Qt6::Gui)
