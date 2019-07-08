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
        b2Vec2 pos = body->GetPosition();
        states.transform.translate({pos.x * m2px, - pos.y * m2px + WIN_SIZE.y});
        states.transform.rotate(- body->GetAngle() * 360.0f / 2.0f / 3.14f);
        target.draw(part, states);
    }
}