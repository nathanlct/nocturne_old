#include "Body.hpp"


Body::Body(const BodyDef& body_def, b2World* world) :
    body(nullptr),
    parts()
{
    b2BodyDef def;
    def.position = b2Vec2(body_def.x, body_def.y);
    def.angle = body_def.angle;
    if(body_def.dynamic) def.type = b2_dynamicBody;
    def.bullet = body_def.bullet;
    def.linearDamping = body_def.linear_damping;
    def.angularDamping = body_def.angular_damping;

    body = world->CreateBody(&def);
}

Body::~Body() {
    body = nullptr;  // TODO need to clean better?
}

void Body::add_part(const BodyPartDef& part_def) {
    BodyPart body_part(part_def, body);
    parts.push_back(body_part);
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const BodyPart& part: parts) {
        sf::Vector2f pos = m2px(body->GetPosition(), true);
        states.transform.translate(pos);
        target.draw(part, states);
    }
}