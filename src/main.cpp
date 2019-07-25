#include <chrono>
#include <thread>

#include "World.hpp"
#include "Body.hpp"
#include "BodyPart.hpp"
#include "RectangleShape.hpp"
#include "Simulation.hpp"


int main()
{
    // create world
    World* world = new World;

    // create body
    BodyDef body_def;
    body_def.x = 15.0f;
    body_def.y = 5.0f;
    body_def.angle = 3.14f / 8.0f;
    Body* body = world->create_body(body_def);

    RectangleShape* shape = new RectangleShape(2.0f, 5.0f);
    BodyPartDef part_def(shape);
    part_def.friction = 1.0f;
    part_def.density = 1.0f;
    part_def.restitution = 1.0f;
    part_def.collides = true;

    body->add_part(part_def);

    // create simulation
    Simulation sim(world);

    // run simulation
    while(sim.isRunning()) {
        sim.step(1.0f / 30.0f);
    }

    // delete objects
    delete world;
    delete body;

	return 0;
}
