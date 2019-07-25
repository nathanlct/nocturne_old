#pragma once

#include "utils.hpp"
#include "World.hpp"

#include <SFML/Window.hpp>

#include <iostream>
#include <sstream>

#include <chrono>
#include <thread>


class Simulation {

public:
    Simulation(World* world);
    bool step(float elapsed);

    bool isRunning();

private:
    bool handleEvents();

private:
    World* world;
    sf::RenderWindow* window;

    sf::View main_view;
    sf::View status_bar_view;

    sf::Font font;

    sf::Vector2f mouse_pos;
    bool dragging;
};