#include "BodyPart.hpp"


BodyPart::BodyPart(const BodyPartDef& body_part_def, b2Body* body) {
    b2FixtureDef def;
    def.shape = body_part_def.shape->get_shape();
    def.density = body_part_def.density;
    def.friction = body_part_def.friction;
    def.restitution = body_part_def.restitution;
    def.isSensor = body_part_def.collides;
    body->CreateFixture(&def);
}

BodyPart::~BodyPart() {

}

void BodyPart::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}