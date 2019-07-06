#pragma once

#include "Box2D/Box2D.h"

#include <SFML/Graphics.hpp>


class Shape : public sf::Drawable {
public:
    Shape();
    ~Shape();

    b2Shape* get();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};