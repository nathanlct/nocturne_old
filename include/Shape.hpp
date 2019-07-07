#pragma once

#include "Box2D/Box2D.h"

#include <SFML/Graphics.hpp>


class Shape : public sf::Drawable {
public:
    Shape();

    b2Shape* get_shape();

protected:
    std::unique_ptr<b2Shape> shape;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};