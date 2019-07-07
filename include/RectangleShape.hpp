#pragma once

#include "Box2D/Box2D.h"

#include "Shape.hpp"

#include <memory>


class RectangleShape : public Shape {
public:
    RectangleShape(float width, float height) :
        width(width), height(height)
    {
        shape = std::make_unique<b2PolygonShape>();
        get_shape()->SetAsBox(width / 2.0f, height / 2.0f);
    }

    b2PolygonShape* get_shape() {
        return reinterpret_cast<b2PolygonShape*>(shape.get());
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {

    }

private:
    float width;
    float height;
};