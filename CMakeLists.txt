cmake_minimum_required(VERSION 3.18)
project(Billiards)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(BUILD_SHARED_LIBS OFF)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

set(SOURCES Billiards/main.cpp Billiards/Game.cpp Billiards/Interface.cpp
        Billiards/physics.cpp Billiards/maths.cpp Billiards/Table.cpp Billiards/Ball.cpp Billiards/Border.cpp)

add_executable(Billiards ${SOURCES})
target_link_libraries(Billiards sfml-graphics sfml-window sfml-system)
