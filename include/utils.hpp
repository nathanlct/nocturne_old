#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>


extern b2Vec2 WORLD_SIZE; // in meters (m)
extern sf::Vector2f WIN_SIZE; // in pixels (px)


extern float m2px;  // the number of pixels used to represent one meter


// need to negate all angles and flip all px positions because y axis
// is pointing down in sfml
// sf::Vector2f m2px(const b2Vec2& v, bool flip=false);

// float m2px(float m);

float rad2deg(float angle, bool flip=false);

float deg2rad(float angle);

b2Vec2 rotate(const b2Vec2& v, float angle);

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
