#include "utils.hpp"


b2Vec2 WORLD_SIZE(40.0f, 40.0f); // in meters (m)
sf::Vector2f WIN_SIZE(1500, 1500); // in pixels (px)

float m2px(50.0f);


float rad2deg(float angle, bool flip) {
    return (flip ? -1 : 1) * angle * 360 / 2 / b2_pi;
}

float deg2rad(float angle) {
    return angle * b2_pi / 180.0f;
}

b2Vec2 rotate(const b2Vec2& v, float angle) {
    float new_x = v.x * cos(angle) - v.y * sin(angle);
    float new_y = v.x * sin(angle) + v.y * cos(angle);
    return b2Vec2(new_x, new_y);
}