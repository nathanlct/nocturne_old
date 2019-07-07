#include "World.hpp"


World::World() :
    world(nullptr),
    bodies(),
    timestep(1.0f / 60.0f),
    velocity_iterations(10),
    position_iterations(8)
{
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);
}

World::~World() {
    delete world;
}

Body* World::create_body(BodyDef body_def) {
    Body* body = new Body(body_def, world);
    bodies.push_back(body);
    return body;
}

void World::step() {
    world->Step(timestep, velocity_iterations, position_iterations);
    world->ClearForces();
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(Body* body: bodies) {
        target.draw(*body, states);
    }
}
