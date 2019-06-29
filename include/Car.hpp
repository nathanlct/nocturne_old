#pragma once

#include "Box2D/Box2D.h"


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "utils.hpp"



inline void kill_orthogonal_velocity(b2Body* body) {
    b2Vec2 velocity = body->GetLinearVelocity();

    b2Vec2 forward_normal = body->GetWorldVector(b2Vec2(1, 0));
    b2Vec2 right_normal = body->GetWorldVector(b2Vec2(0, -1));

    b2Vec2 forward_velocity = b2Dot(forward_normal, velocity) * forward_normal;
    b2Vec2 lateral_velocity = b2Dot(right_normal, velocity) * right_normal;

    body->SetLinearVelocity(forward_velocity + 0.1 * lateral_velocity);

    // other option is to apply an impulse in the opposite direction of the lateral velocity
    // b2Vec2 impulse = -lateral_velocity * body->GetMass();
}


struct CarDef {
    // everything position and dimension is in meters 
    // every angle is in radians

    CarDef() {
        car_starting_pos = b2Vec2(0.0f, 0.0f);
        car_starting_angle = 0.0f;  // FIXME bug if this is not 0 (wrong wheels creation)

        skeleton_size = b2Vec2(2.0f, 1.0f);
        wheel_size = b2Vec2(0.6f, 0.3f);

        left_front_wheel_pos = b2Vec2(1.0f, 0.5f);
        right_front_wheel_pos = b2Vec2(1.0f, -0.5f);
        left_rear_wheel_pos = b2Vec2(-0.8f, -0.5f);
        right_rear_wheel_pos = b2Vec2(-0.8f, 0.5f);

        linear_damping = 1.0f;
        angular_damping = 1.0f;
        bullet = true;

        density = 3.0f;
        friction = 0.2f;
        restitution = 0.1f;

        remove_wheels_collisions = false;
    }

    b2Vec2 car_starting_pos; // pos is center of car
    float car_starting_angle;

    b2Vec2 skeleton_size;
    b2Vec2 wheel_size;

    b2Vec2 left_front_wheel_pos;
    b2Vec2 right_front_wheel_pos;
    b2Vec2 left_rear_wheel_pos;
    b2Vec2 right_rear_wheel_pos;

    float linear_damping;
    float angular_damping;
    bool bullet;

    float density;
    float friction;
    float restitution;

    bool remove_wheels_collisions;
};

class Car : public sf::Drawable {
public:
    Car(const CarDef& car_def, b2World* world);

public:
    void update(float engine_speed, float steering_angle);

private:
    b2Body* skeleton;
    b2Body* left_front_wheel;
    b2Body* right_front_wheel;
    b2Body* left_rear_wheel;
    b2Body* right_rear_wheel;

    b2RevoluteJoint* left_front_wheel_joint;
    b2RevoluteJoint* right_front_wheel_joint;

    b2Vec2 skeleton_size;
    b2Vec2 wheel_size;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};