cmake_minimum_required(VERSION 2.8.9)
project(nocturne)
 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(PROJECT_LINK_LIBS Box2D.a libsfml-audio.dylib libsfml-graphics.dylib 
        libsfml-network.dylib libsfml-system.dylib libsfml-window.dylib)
else()
    set(PROJECT_LINK_LIBS Box2D.a libsfml-audio.so libsfml-graphics.so 
        libsfml-network.so libsfml-system.so libsfml-window.so)
endif()

link_directories(/usr/lib /usr/local/lib)
include_directories(include /usr/include /usr/local/include)

file(GLOB SOURCES "src/*.cpp")
 
add_executable(nocturne ${SOURCES})
target_link_libraries(nocturne ${PROJECT_LINK_LIBS})

