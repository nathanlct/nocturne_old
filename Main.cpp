#include "Box2D/Box2D.h"


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>



b2Vec2 WORLD_SIZE(30.0f, 30.0f); // in meters (m)
sf::Vector2f WIN_SIZE(1500, 1500); // in pixels (px)


// need to negate all angles and flip all px positions because y axis
// is pointing down in sfml
sf::Vector2f m2px(const b2Vec2& v, bool flip=false) {
    return { v.x * WIN_SIZE.x / WORLD_SIZE.x,
             (flip ? WIN_SIZE.y : 0) + (flip ? -1 : 1) * v.y * WIN_SIZE.y / WORLD_SIZE.y  };
}

float rad2deg(float angle, bool flip=false) {
    return (flip ? -1 : 1) * angle * 360 / 2 / b2_pi;
}

float deg2rad(float angle) {
    return angle * b2_pi / 180.0f;
}

void kill_orthogonal_velocity(b2Body* body) {
    b2Vec2 velocity = body->GetLinearVelocity();

    b2Vec2 forward_normal = body->GetWorldVector(b2Vec2(0, 1));
    // b2Vec2 right_normal = body->GetWorldVector(b2Vec2(1, 0));

    b2Vec2 forward_velocity = b2Dot(forward_normal, velocity) * forward_normal;
    // b2Vec2 lateral_velocity = b2Dot(right_normal, velocity) * right_normal;

    body->SetLinearVelocity(forward_velocity);

    // other option is to apply an impulse in the opposite direction of the lateral velocity
    // b2Vec2 impulse = -lateral_velocity * body->GetMass();
    // this way we can not entirely remove it

}


int main()
{
	b2Vec2 gravity(0.0f, 0.0f);
    b2World* world = new b2World(gravity);

    float32 timestep = 1.0f / 60.0f;
    int32 velocity_iterations = 10;
    int32 position_iterations = 8;

    // car dimensions and stuff
    b2Vec2 skeleton_size(1.0f, 2.0f);
    b2Vec2 wheel_size(0.3f, 0.6f);

    b2Vec2 left_front_wheel_pos(-0.5f, 0.6f);
    b2Vec2 right_front_wheel_pos(0.5f, 0.6f);
    b2Vec2 left_rear_wheel_pos(-0.5f, -0.6f);
    b2Vec2 right_rear_wheel_pos(0.5f, -0.6f);

    // car starting position (center of car, m) and angle (clockwise, rad)
    b2Vec2 car_starting_pos = 0.5f * WORLD_SIZE; // b2Vec2(15.0f, 15.0f);
    float car_starting_angle = 0.0f;

    // define the parts of the car
    b2BodyDef skeleton_def;
    skeleton_def.type = b2_dynamicBody;
    skeleton_def.position = car_starting_pos;
    skeleton_def.angle = car_starting_angle;
    skeleton_def.linearDamping = 1.0f;
    skeleton_def.angularDamping = 1.0f;
    skeleton_def.bullet = true;

    b2BodyDef left_front_wheel_def;
    left_front_wheel_def.type = b2_dynamicBody;
    left_front_wheel_def.position = car_starting_pos + left_front_wheel_pos;
    left_front_wheel_def.angle = car_starting_angle;

    b2BodyDef right_front_wheel_def;
    right_front_wheel_def.type = b2_dynamicBody;
    right_front_wheel_def.position = car_starting_pos + right_front_wheel_pos;
    right_front_wheel_def.angle = car_starting_angle;

    b2BodyDef left_rear_wheel_def;
    left_rear_wheel_def.type = b2_dynamicBody;
    left_rear_wheel_def.position = car_starting_pos + left_rear_wheel_pos;
    left_rear_wheel_def.angle = car_starting_angle;

    b2BodyDef right_rear_wheel_def;
    right_rear_wheel_def.type = b2_dynamicBody;
    right_rear_wheel_def.position = car_starting_pos + right_rear_wheel_pos;
    right_rear_wheel_def.angle = car_starting_angle;

    // create the car parts
    b2Body* skeleton = world->CreateBody(&skeleton_def);
    b2Body* left_front_wheel = world->CreateBody(&left_front_wheel_def);
    b2Body* right_front_wheel = world->CreateBody(&right_front_wheel_def);
    b2Body* left_rear_wheel = world->CreateBody(&left_rear_wheel_def);
    b2Body* right_rear_wheel = world->CreateBody(&right_rear_wheel_def);

    // define the shape of the car parts
    b2PolygonShape skeleton_shape;
    skeleton_shape.SetAsBox(skeleton_size.x / 2.0f, skeleton_size.y / 2.0f);

    b2PolygonShape wheel_shape;
    wheel_shape.SetAsBox(wheel_size.x / 2.0f, wheel_size.y / 2.0f);

    // link shapes to bodies
    b2FixtureDef skeleton_fixture_def;
    skeleton_fixture_def.shape = &skeleton_shape;
    skeleton_fixture_def.density = 1.0f;
    skeleton_fixture_def.friction = 0.2f;
    skeleton_fixture_def.restitution = 0.1f; // bounciness
    skeleton->CreateFixture(&skeleton_fixture_def);

    b2FixtureDef wheel_fixture_def;
    wheel_fixture_def.shape = &wheel_shape;
    wheel_fixture_def.density = 1.0f;
    wheel_fixture_def.friction = 0.2f;
    skeleton_fixture_def.restitution = 0.1f; // bounciness
    skeleton_fixture_def.isSensor = true; // wheels do not participate in collisions
    left_front_wheel->CreateFixture(&wheel_fixture_def);
    right_front_wheel->CreateFixture(&wheel_fixture_def);
    left_rear_wheel->CreateFixture(&wheel_fixture_def);
    right_rear_wheel->CreateFixture(&wheel_fixture_def);

    // create joints for front wheels
    b2RevoluteJointDef left_front_wheel_joint_def;
    left_front_wheel_joint_def.Initialize(skeleton, left_front_wheel, left_front_wheel->GetWorldCenter());
    // left_front_wheel_joint_def.enableLimit = true;
    // left_front_wheel_joint_def.lowerAngle = -0.4 * b2_pi;
    // left_front_wheel_joint_def.upperAngle = 0.4 * b2_pi;
    left_front_wheel_joint_def.enableMotor = true;
    left_front_wheel_joint_def.maxMotorTorque = 100.0f;
    b2RevoluteJoint* left_front_wheel_joint = (b2RevoluteJoint*)world->CreateJoint(&left_front_wheel_joint_def);

    b2RevoluteJointDef right_front_wheel_joint_def;
    right_front_wheel_joint_def.Initialize(skeleton, right_front_wheel, right_front_wheel->GetWorldCenter());
    // right_front_wheel_joint_def.enableLimit = true;
    // right_front_wheel_joint_def.lowerAngle = -0.4 * b2_pi;
    // right_front_wheel_joint_def.upperAngle = 0.4 * b2_pi;
    right_front_wheel_joint_def.enableMotor = true;
    right_front_wheel_joint_def.maxMotorTorque = 100.0f;
    b2RevoluteJoint* right_front_wheel_joint = (b2RevoluteJoint*)world->CreateJoint(&right_front_wheel_joint_def);

    // create joints for rear wheels
    b2PrismaticJointDef left_rear_wheel_joint_def;
    left_rear_wheel_joint_def.Initialize(skeleton, left_rear_wheel, left_rear_wheel->GetWorldCenter(), b2Vec2(1,0));
    left_rear_wheel_joint_def.enableLimit = true;
    left_rear_wheel_joint_def.lowerTranslation = 0.0f;
    left_rear_wheel_joint_def.upperTranslation = 0.0f;
    left_rear_wheel_joint_def.enableMotor = false;
    world->CreateJoint(&left_rear_wheel_joint_def);

    b2PrismaticJointDef right_rear_wheel_joint_def;
    right_rear_wheel_joint_def.Initialize(skeleton, right_rear_wheel, right_rear_wheel->GetWorldCenter(), b2Vec2(1,0));
    right_rear_wheel_joint_def.enableLimit = true;
    right_rear_wheel_joint_def.lowerTranslation = 0.0f;
    right_rear_wheel_joint_def.upperTranslation = 0.0f;
    right_rear_wheel_joint_def.enableMotor = false;
    world->CreateJoint(&right_rear_wheel_joint_def);

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
            sf::Time elapsed = clock.restart();

            // update forces etc
            kill_orthogonal_velocity(left_front_wheel);
            kill_orthogonal_velocity(right_front_wheel);
            kill_orthogonal_velocity(left_rear_wheel);
            kill_orthogonal_velocity(right_rear_wheel);

            float max_engine_speed = 30.0f;
            float max_steering_angle = deg2rad(60.0f);

            float engine_speed = 0;
            float steering_angle = 0;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) engine_speed = max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) engine_speed = -max_engine_speed;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) steering_angle = max_steering_angle;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) steering_angle = -max_steering_angle;

            // driving
            b2Vec2 left_wheel_forward_normal = left_front_wheel->GetWorldVector(b2Vec2(0, 1));
            left_front_wheel->ApplyForce(engine_speed * left_wheel_forward_normal, left_front_wheel->GetWorldCenter(), true);
            b2Vec2 right_wheel_forward_normal = right_front_wheel->GetWorldVector(b2Vec2(0, 1));
            right_front_wheel->ApplyForce(engine_speed * right_wheel_forward_normal, right_front_wheel->GetWorldCenter(), true);

            // steering
            float steering_speed = 1.0f;
            if(steering_angle == 0.0f) steering_speed *= 10.0f;
            float left_delta_angle = steering_angle - left_front_wheel_joint->GetJointAngle();
            left_front_wheel_joint->SetMotorSpeed(steering_speed * left_delta_angle);
            float right_delta_angle = steering_angle - right_front_wheel_joint->GetJointAngle();
            right_front_wheel_joint->SetMotorSpeed(steering_speed * right_delta_angle);

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

            // draw skeleton
            sf::RectangleShape skeleton_rect(m2px(skeleton_size));
            skeleton_rect.setOrigin(skeleton_rect.getSize() / 2.0f);
            skeleton_rect.setPosition(m2px(skeleton->GetPosition(), true));
            skeleton_rect.setRotation(rad2deg(skeleton->GetAngle(), true));
            skeleton_rect.setFillColor(sf::Color::Transparent);
            skeleton_rect.setOutlineColor(sf::Color::Red);
            skeleton_rect.setOutlineThickness(1.0f);
            window.draw(skeleton_rect);

            // draw wheels
            for(auto wheel : {left_front_wheel, right_front_wheel, left_rear_wheel, right_rear_wheel}) 
            {
                sf::RectangleShape wheel_rect(m2px(wheel_size));
                wheel_rect.setOrigin(wheel_rect.getSize() / 2.0f);
                wheel_rect.setPosition(m2px(wheel->GetPosition(), true));
                wheel_rect.setRotation(rad2deg(wheel->GetAngle(), true));
                wheel_rect.setFillColor(sf::Color::Transparent);
                wheel_rect.setOutlineColor(sf::Color::Red);
                wheel_rect.setOutlineThickness(1.0f);
                window.draw(wheel_rect);
            }

            window.display();
        }
    }

    delete world;

	return 0;
}
