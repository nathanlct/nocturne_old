#pragma once

#include "Box2D/Box2D.h"

#include <SFML/Graphics.hpp>

#include "Shape.hpp"


struct BodyPartDef {
    BodyPartDef(Shape* shape) :
        shape(shape),
        density(1.0f), friction(1.0f), restitution(1.0f),
        collides(true)
    {}

    Shape* shape;
    float density;
    float friction;
    float restitution;
    bool collides;
};


class BodyPart : public sf::Drawable {
public:
    BodyPart(const BodyPartDef& body_part_def, b2Body* body);
    ~BodyPart();

private:
    Shape* shape;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};