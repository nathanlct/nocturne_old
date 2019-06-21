#include "utils.hpp"


b2Vec2 WORLD_SIZE(40.0f, 40.0f); // in meters (m)
sf::Vector2f WIN_SIZE(1500, 1500); // in pixels (px)



// need to negate all angles and flip all px positions because y axis
// is pointing down in sfml
sf::Vector2f m2px(const b2Vec2& v, bool flip) {
    return { v.x * WIN_SIZE.x / WORLD_SIZE.x,
             (flip ? WIN_SIZE.y : 0) + (flip ? -1 : 1) * v.y * WIN_SIZE.y / WORLD_SIZE.y  };
}

float m2px(float m) {
    assert(WIN_SIZE.x / WORLD_SIZE.x == WIN_SIZE.y / WORLD_SIZE.y);
    return m * WIN_SIZE.x / WORLD_SIZE.x;
}

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