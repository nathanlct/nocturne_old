#include "Box2D/Box2D.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "World.hpp"
#include "Body.hpp"
#include "BodyPart.hpp"
#include "utils.hpp"
#include "RectangleShape.hpp"



int main()
{
    World world; // TODO put world size as parameter of ctor

    BodyDef body_def;
    body_def.x = 20.0f;
    body_def.y = 30.0f;
    body_def.angle = 3.14f / 4.0f;
    Body* body = world.create_body(body_def);
    RectangleShape* shape = new RectangleShape(2.0f, 5.0f);
    BodyPartDef part_def(shape);
    body->add_part(part_def);

    sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), "Project Magenta");
    constexpr float max_fps = 60;
    const float time_between_frames = 1.0 / max_fps;

    sf::Clock clock;
    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        if(clock.getElapsedTime().asSeconds() >= time_between_frames) {
            window.clear(sf::Color::White);
            float elapsed = clock.getElapsedTime().asSeconds();
            clock.restart();

            world.step();
            window.draw(world);      

            window.display();
        }
    }

	return 0;
}
