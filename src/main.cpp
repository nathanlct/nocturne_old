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
    body_def.x = 15.0f;
    body_def.y = 15.0f;
    body_def.angle = 3.14f / 4.0f;
    Body* body = world.create_body(body_def);
    RectangleShape* shape = new RectangleShape(2.0f, 5.0f);
    BodyPartDef part_def(shape);
    body->add_part(part_def);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), "Project Magenta", sf::Style::Default, settings);
    
    constexpr float max_fps = 60;
    const float time_between_frames = 1.0 / max_fps;

    sf::View main_view(sf::FloatRect(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y));
    // main_view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
    main_view.zoom(-1.0f); // flip because sfml y axis goes from top to bottom

    sf::Vector2f mouse_pos(0.0f, 0.0f);
    bool dragging = false;

    sf::Clock clock;
    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    main_view.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
                    break;
                case sf::Event::MouseWheelScrolled:
                    if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        float delta = event.mouseWheelScroll.delta;
                        main_view.zoom(1 + delta / 100.0f);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                        dragging = true;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        dragging = false;
                    }
                    break;
                case sf::Event::MouseMoved:
                    if(dragging) {
                        sf::Vector2f new_mouse_pos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
                        main_view.move(new_mouse_pos - mouse_pos);
                        mouse_pos = new_mouse_pos;
                    }
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

            window.setView(main_view);
            window.draw(world);      



            window.display();
        }
    }

	return 0;
}
