#pragma once

#include "Box2D/Box2D.h"

#include <SFML/Graphics.hpp>

#include "Body.hpp"


class World : public sf::Drawable {
public:
    World();
    ~World();

    Body* create_body(BodyDef body_def);
    void step();

private:
    b2World* world;
    std::vector<Body*> bodies;

    float timestep;
    int velocity_iterations;
    int position_iterations;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};