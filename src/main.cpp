#include "Box2D/Box2D.h"


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Car.hpp"




int main()
{
	b2Vec2 gravity(0.0f, 0.0f);
    b2World* world = new b2World(gravity);

    float32 timestep = 1.0f / 60.0f;
    int32 velocity_iterations = 10;
    int32 position_iterations = 8;

    // create obstacles
    b2BodyDef chain_def;
    b2Body* chain = world->CreateBody(&chain_def);

    b2Vec2 chain_vertices[12];
    chain_vertices[0].Set(-50.0f, 15.0f);
    chain_vertices[1].Set(-50.0f, 25.0f);
    chain_vertices[2].Set(15.0f, 25.0f);
    chain_vertices[3].Set(15.0f, 90.0f);
    chain_vertices[4].Set(25.0f, 90.0f);
    chain_vertices[5].Set(25.0f, 25.0f);
    chain_vertices[6].Set(90.0f, 25.0f);
    chain_vertices[7].Set(90.0f, 15.0f);
    chain_vertices[8].Set(25.0f, 15.0f);
    chain_vertices[9].Set(25.0f, -50.0f);
    chain_vertices[10].Set(15.0f, -50.0f);
    chain_vertices[11].Set(15.0f, 15.0f);
    b2ChainShape chain_shape;
    chain_shape.CreateLoop(chain_vertices, 12);

    b2FixtureDef chain_fixture_def;
    chain_fixture_def.shape = &chain_shape;
    chain_fixture_def.friction = 1.0f;
    chain_fixture_def.restitution = 0.1f;
    b2Fixture* chain_fixture = (b2Fixture*)chain->CreateFixture(&chain_fixture_def);




    sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), "Project Magenta");
    constexpr float max_fps = 60;
    const float time_between_frames = 1.0 / max_fps;


    float dt_car_spawn = 0.0f;
    std::vector<Car> cars;

    sf::Clock clock;
    while(window.isOpen()) {
        // retrieve events
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                        case sf::Keyboard::Space:
                            // skeleton->SetPosition(car_starting_pos);
                            // skeleton->SetAngle(car_starting_angle);
                            break;
                        default: break;
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

            if(dt_car_spawn <= 0.0f) {
                CarDef car_def;

                car_def.car_starting_pos = b2Vec2(-10.0f, 17.5f);
                car_def.car_starting_angle = 0.0f;
                {Car car(car_def, world);
                cars.push_back(car);}
                
                car_def.car_starting_pos = b2Vec2(50.0f, 22.5f);
                car_def.car_starting_angle = b2_pi;
                {Car car(car_def, world);
                cars.push_back(car);}

                car_def.car_starting_pos = b2Vec2(22.5f, -10.0f);
                car_def.car_starting_angle = b2_pi / 2.0f;
                {Car car(car_def, world);
                cars.push_back(car);}

                car_def.car_starting_pos = b2Vec2(17.5f, 50.0f);
                car_def.car_starting_angle = - b2_pi / 2.0f;
                {Car car(car_def, world);
                cars.push_back(car);}

                dt_car_spawn = 1.0f;
            }
            dt_car_spawn -= elapsed;

            float engine_speed = 0;
            float steering_angle = 0;

            for(auto car: cars)
                car.update(50.0f, 0.0f);

            // physics step
            world->Step(timestep, velocity_iterations, position_iterations);
            world->ClearForces();

            for(auto car: cars)
                window.draw(car);

            // draw obstacles
            b2Vec2* vertices = ((b2ChainShape*)chain_fixture->GetShape())->m_vertices;
            int n_vertices = ((b2ChainShape*)chain_fixture->GetShape())->m_count;

            sf::Vertex* line = new sf::Vertex[n_vertices];
            for(int i = 0; i < n_vertices; ++i) {
                line[i] = sf::Vertex(m2px(vertices[i], true), sf::Color::Black);
            }

            window.draw(line, n_vertices, sf::LinesStrip);

            window.display();
        }
    }

    delete world;

	return 0;
}
