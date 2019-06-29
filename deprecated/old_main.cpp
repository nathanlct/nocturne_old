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

    CarDef car_def;
    car_def.car_starting_pos = 0.5f * WORLD_SIZE;

    Car car(car_def, world);

    car_def.car_starting_pos = 0.2f * WORLD_SIZE;
    Car car2(car_def, world);

    // create obstacles
    std::vector<b2Body*> obstacle_balls;

    float obstacle_ball_radius = 0.2f;
    for(float pos_x = 0.0f; pos_x <= 30.0f; pos_x += 5.0f * obstacle_ball_radius) {
        for(float pos_y = 0.0f; pos_y <= 30.0f; pos_y += 5.0f * obstacle_ball_radius) {
            b2BodyDef obstacle_ball_def;
            obstacle_ball_def.type = b2_dynamicBody;
            obstacle_ball_def.position = b2Vec2(pos_x, pos_y);
            obstacle_ball_def.linearDamping = 1.5f;
            obstacle_ball_def.angularDamping = 1.0f;

            b2Body* obstacle_ball = world->CreateBody(&obstacle_ball_def);
            obstacle_balls.push_back(obstacle_ball);

            b2CircleShape obstacle_ball_shape;
            obstacle_ball_shape.m_radius = obstacle_ball_radius;
            
            b2FixtureDef obstacle_ball_fixture_def;
            obstacle_ball_fixture_def.shape = &obstacle_ball_shape;
            obstacle_ball_fixture_def.density = 1.5f;
            obstacle_ball_fixture_def.friction = 2.0f;
            obstacle_ball_fixture_def.restitution = 1.0f;
            obstacle_ball->CreateFixture(&obstacle_ball_fixture_def);
        }
    }

    b2BodyDef chain_def;
    b2Body* chain = world->CreateBody(&chain_def);

    b2Vec2 chain_vertices[5];
    chain_vertices[0].Set(0.0f, 0.0f);
    chain_vertices[1].Set(0.0f, 30.0f);
    chain_vertices[2].Set(30.0f, 30.0f);
    chain_vertices[3].Set(30.0f, 0.0f);
    chain_vertices[4].Set(0.0f, 0.0f);
    b2ChainShape chain_shape;
    chain_shape.CreateChain(chain_vertices, 5);

    b2FixtureDef chain_fixture_def;
    chain_fixture_def.shape = &chain_shape;
    chain_fixture_def.friction = 1.0f;
    chain_fixture_def.restitution = 1.0f;
    b2Fixture* chain_fixture = (b2Fixture*)chain->CreateFixture(&chain_fixture_def);




    sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), "Project Magenta");
    constexpr float max_fps = 60;
    const float time_between_frames = 1.0 / max_fps;


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
            clock.restart();

            float max_engine_speed = 60.0f;
            float max_steering_angle = deg2rad(60.0f);

            float engine_speed = 0;
            float steering_angle = 0;
            float engine_speed2 = 0;
            float steering_angle2 = 0;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) engine_speed = max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) engine_speed = -max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) steering_angle = max_steering_angle;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) steering_angle = -max_steering_angle;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) engine_speed2 = max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) engine_speed2 = -max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) steering_angle2 = max_steering_angle;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) steering_angle2 = -max_steering_angle;

            car.update(engine_speed, steering_angle);
            car2.update(engine_speed2, steering_angle2);

            // physics step
            world->Step(timestep, velocity_iterations, position_iterations);
            world->ClearForces();

            // draw grid
            for(float x = 0.0f; x < WORLD_SIZE.x; x += 1.0f) {
                sf::Vertex line[] =
                {
                    sf::Vertex(m2px(b2Vec2(x, 0.0f), true), sf::Color(0, 0, 0, 50)),
                    sf::Vertex(m2px(b2Vec2(x, WORLD_SIZE.y), true), sf::Color(0, 0, 0, 50))
                };
                window.draw(line, 2, sf::Lines);
            }
            for(float y = 0.0f; y < WORLD_SIZE.y; y += 1.0f) {
                sf::Vertex line[] =
                {
                    sf::Vertex(m2px(b2Vec2(0.0f, y), true), sf::Color(0, 0, 0, 50)),
                    sf::Vertex(m2px(b2Vec2(WORLD_SIZE.x, y), true), sf::Color(0, 0, 0, 50))
                };
                window.draw(line, 2, sf::Lines);
            }

            window.draw(car);
            window.draw(car2);

            // draw obstacles
            for(b2Body* obstacle_ball : obstacle_balls) {
                sf::CircleShape obstacle_ball_circle(m2px(obstacle_ball_radius));
                obstacle_ball_circle.setOrigin(m2px(obstacle_ball_radius), m2px(obstacle_ball_radius));
                obstacle_ball_circle.setPosition(m2px(obstacle_ball->GetPosition(), true));
                obstacle_ball_circle.setFillColor(sf::Color::Transparent);
                obstacle_ball_circle.setOutlineColor(sf::Color::Blue);
                obstacle_ball_circle.setOutlineThickness(1.0f);
                window.draw(obstacle_ball_circle);
            }

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
