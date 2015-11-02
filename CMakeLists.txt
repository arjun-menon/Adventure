cmake_minimum_required(VERSION 3.2)
project(Adventure)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

set(SOURCE_FILES
        src/PlatformInterface.cpp
        src/Elements.cpp
        src/EntityMap.cpp
        src/PhysicsMap.cpp
        src/SideScrollingView.cpp
        src/GameMain.cpp
    )

add_executable(Adventure ${SOURCE_FILES})

set_property(TARGET Adventure PROPERTY CXX_STANDARD 14)
set_property(TARGET Adventure PROPERTY CXX_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(SDL2 SDL2)
    if (NOT SDL2)
        message(FATAL_ERROR "SDL2 not found.")
    endif()
endif()

target_link_libraries(Adventure ${SDL2})
