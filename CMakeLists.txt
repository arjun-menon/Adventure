cmake_minimum_required(VERSION 3.2)
project(Adventure)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(SOURCE_FILES
        src/PlatformInterface.cpp
        src/Elements.cpp
        src/EntityMap.cpp
        src/PhysicsMap.cpp
        src/SideScrollingView.cpp
        src/GameMain.cpp
    )

add_executable(Adventure ${SOURCE_FILES})

target_link_libraries(Adventure sfml-graphics sfml-window sfml-system)