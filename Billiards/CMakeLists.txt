cmake_minimum_required(VERSION 3.20)
project(Billiards)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML"
    GIT_TAG 2f11710abc5aa478503a7ff3f9e654bd2078ebab
)
message(STATUS "Fetching SFML...")

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

FetchContent_MakeAvailable(sfml)

add_executable(Main
    # main_graphic.cpp
    main.cpp
    globals.cpp objects.cpp physics.cpp)
target_link_libraries(Main PRIVATE sfml-graphics)
#todo sfml-window etc
