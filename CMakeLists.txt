cmake_minimum_required(VERSION 2.8.9)
project(nocturne)
 
set(PROJECT_LINK_LIBS Box2D.a libsfml-audio.so libsfml-graphics.so libsfml-network.so libsfml-system.so libsfml-window.so)
link_directories(/usr/lib)

include_directories(include /usr/include /home/fangyu/GitHub/Box2D)

file(GLOB SOURCES "src/*.cpp")
 
add_executable(nocturne ${SOURCES})
target_link_libraries(nocturne ${PROJECT_LINK_LIBS})

