#pragma once

#include "Box2D/Box2D.h"
#include <SFML/System/Vector2.hpp>


extern b2Vec2 WORLD_SIZE; // in meters (m)
extern sf::Vector2f WIN_SIZE; // in pixels (px)



// need to negate all angles and flip all px positions because y axis
// is pointing down in sfml
sf::Vector2f m2px(const b2Vec2& v, bool flip=false);

float m2px(float m);

float rad2deg(float angle, bool flip=false);

float deg2rad(float angle);

b2Vec2 rotate(const b2Vec2& v, float angle);