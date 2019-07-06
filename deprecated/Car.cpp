#include "Car.hpp"




Car::Car(const CarDef& car_def, b2World* world) {
    skeleton_size = car_def.skeleton_size;
    wheel_size = car_def.wheel_size;

    // define the parts of the car
    b2BodyDef skeleton_def;
    skeleton_def.type = b2_dynamicBody;
    skeleton_def.position = car_def.car_starting_pos;
    skeleton_def.angle = car_def.car_starting_angle;
    skeleton_def.linearDamping = car_def.linear_damping;
    skeleton_def.angularDamping = car_def.angular_damping;
    skeleton_def.bullet = car_def.bullet;

    b2BodyDef left_front_wheel_def;
    left_front_wheel_def.type = b2_dynamicBody;
    left_front_wheel_def.position = car_def.car_starting_pos + rotate(car_def.left_front_wheel_pos, car_def.car_starting_angle);
    left_front_wheel_def.angle = car_def.car_starting_angle;

    b2BodyDef right_front_wheel_def;
    right_front_wheel_def.type = b2_dynamicBody;
    right_front_wheel_def.position = car_def.car_starting_pos + rotate(car_def.right_front_wheel_pos, car_def.car_starting_angle);
    right_front_wheel_def.angle = car_def.car_starting_angle;

    b2BodyDef left_rear_wheel_def;
    left_rear_wheel_def.type = b2_dynamicBody;
    left_rear_wheel_def.position = car_def.car_starting_pos + rotate(car_def.left_rear_wheel_pos, car_def.car_starting_angle);
    left_rear_wheel_def.angle = car_def.car_starting_angle;

    b2BodyDef right_rear_wheel_def;
    right_rear_wheel_def.type = b2_dynamicBody;
    right_rear_wheel_def.position = car_def.car_starting_pos + rotate(car_def.right_rear_wheel_pos, car_def.car_starting_angle);
    right_rear_wheel_def.angle = car_def.car_starting_angle;

    // create the car parts
    skeleton = world->CreateBody(&skeleton_def);
    left_front_wheel = world->CreateBody(&left_front_wheel_def);
    right_front_wheel = world->CreateBody(&right_front_wheel_def);
    left_rear_wheel = world->CreateBody(&left_rear_wheel_def);
    right_rear_wheel = world->CreateBody(&right_rear_wheel_def);

    // define the shape of the car parts
    b2PolygonShape skeleton_shape;
    skeleton_shape.SetAsBox(car_def.skeleton_size.x / 2.0f, car_def.skeleton_size.y / 2.0f);

    b2PolygonShape wheel_shape;
    wheel_shape.SetAsBox(car_def.wheel_size.x / 2.0f, car_def.wheel_size.y / 2.0f);

    // link shapes to bodies
    b2FixtureDef skeleton_fixture_def;
    skeleton_fixture_def.shape = &skeleton_shape;
    skeleton_fixture_def.density = car_def.density;
    skeleton_fixture_def.friction = car_def.friction;
    skeleton_fixture_def.restitution = car_def.restitution;
    skeleton->CreateFixture(&skeleton_fixture_def);

    b2FixtureDef wheel_fixture_def;
    wheel_fixture_def.shape = &wheel_shape;
    wheel_fixture_def.density = car_def.density;
    wheel_fixture_def.friction = car_def.friction;
    skeleton_fixture_def.restitution = car_def.restitution;
    skeleton_fixture_def.isSensor = car_def.remove_wheels_collisions;
    left_front_wheel->CreateFixture(&wheel_fixture_def);
    right_front_wheel->CreateFixture(&wheel_fixture_def);
    left_rear_wheel->CreateFixture(&wheel_fixture_def);
    right_rear_wheel->CreateFixture(&wheel_fixture_def);

    // create joints for front wheels
    b2RevoluteJointDef left_front_wheel_joint_def;
    left_front_wheel_joint_def.Initialize(skeleton, left_front_wheel, left_front_wheel->GetWorldCenter());
    left_front_wheel_joint_def.enableMotor = true;
    left_front_wheel_joint_def.maxMotorTorque = 100.0f;
    left_front_wheel_joint = (b2RevoluteJoint*)world->CreateJoint(&left_front_wheel_joint_def);

    b2RevoluteJointDef right_front_wheel_joint_def;
    right_front_wheel_joint_def.Initialize(skeleton, right_front_wheel, right_front_wheel->GetWorldCenter());
    right_front_wheel_joint_def.enableMotor = true;
    right_front_wheel_joint_def.maxMotorTorque = 100.0f;
    right_front_wheel_joint = (b2RevoluteJoint*)world->CreateJoint(&right_front_wheel_joint_def);

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
}

void Car::update(float engine_speed, float steering_angle) {
    // update forces etc
    kill_orthogonal_velocity(left_front_wheel);
    kill_orthogonal_velocity(right_front_wheel);
    kill_orthogonal_velocity(left_rear_wheel);
    kill_orthogonal_velocity(right_rear_wheel);

    // driving
    b2Vec2 left_wheel_forward_normal = left_front_wheel->GetWorldVector(b2Vec2(1, 0));
    left_front_wheel->ApplyForce(engine_speed * left_wheel_forward_normal, left_front_wheel->GetWorldCenter(), true);
    b2Vec2 right_wheel_forward_normal = right_front_wheel->GetWorldVector(b2Vec2(1, 0));
    right_front_wheel->ApplyForce(engine_speed * right_wheel_forward_normal, right_front_wheel->GetWorldCenter(), true);

    // steering
    float steering_speed = 1.0f;
    if(steering_angle == 0.0f) steering_speed *= 10.0f;
    float left_delta_angle = steering_angle - left_front_wheel_joint->GetJointAngle();
    left_front_wheel_joint->SetMotorSpeed(steering_speed * left_delta_angle);
    float right_delta_angle = steering_angle - right_front_wheel_joint->GetJointAngle();
    right_front_wheel_joint->SetMotorSpeed(steering_speed * right_delta_angle);
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // draw wheels
    for(b2Body* wheel : {left_front_wheel, right_front_wheel, left_rear_wheel, right_rear_wheel}) 
    {
        sf::RectangleShape wheel_rect(m2px(wheel_size));
        wheel_rect.setOrigin(wheel_rect.getSize() / 2.0f);
        wheel_rect.setPosition(m2px(wheel->GetPosition(), true));
        wheel_rect.setRotation(rad2deg(wheel->GetAngle(), true));
        wheel_rect.setFillColor(sf::Color::Transparent);
        wheel_rect.setOutlineColor(sf::Color::Red);
        wheel_rect.setOutlineThickness(1.0f);
        target.draw(wheel_rect, states);
    }
    
    // draw skeleton
    sf::RectangleShape skeleton_rect(m2px(skeleton_size));
    skeleton_rect.setOrigin(skeleton_rect.getSize() / 2.0f);
    skeleton_rect.setPosition(m2px(skeleton->GetPosition(), true));
    skeleton_rect.setRotation(rad2deg(skeleton->GetAngle(), true));
    skeleton_rect.setFillColor(sf::Color::Transparent);
    skeleton_rect.setOutlineColor(sf::Color::Red);
    skeleton_rect.setOutlineThickness(1.0f);
    target.draw(skeleton_rect, states);
}