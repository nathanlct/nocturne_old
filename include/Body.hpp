#pragma once

#include "Box2D/Box2D.h"

#include <SFML/Graphics.hpp>

#include "BodyPart.hpp"


struct BodyDef {
    BodyDef() :
        x(0.0f), y(0.0f), angle(0.0f),
        dynamic(true), bullet(false),
        linear_damping(1.0f), angular_damping(1.0f)
    {}

    float x;
    float y;
    float angle;
    bool dynamic;
    bool bullet;
    float linear_damping;
    float angular_damping;
};


class Body : public sf::Drawable {
public:
    Body(const BodyDef& body_def, b2World* world);
    ~Body();

    void add_part(const BodyPartDef& part_def);

private:
    b2Body* body;
    std::vector<BodyPart> parts;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};