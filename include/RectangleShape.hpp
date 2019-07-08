#pragma once

#include "Box2D/Box2D.h"

#include "Shape.hpp"
#include "utils.hpp"

#include <memory>
#include <iostream>


class RectangleShape : public Shape {
public:
    RectangleShape(float width, float height) :
        width(width), height(height)
    {
        shape = std::make_unique<b2PolygonShape>();
        get_shape()->SetAsBox(width / 2.0f, height / 2.0f);

        sf::Vector2f rect_size(width * m2px, height * m2px);
        rect = sf::RectangleShape(rect_size);
        rect.setOrigin(rect_size / 2.0f);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1.0f);
    }

    b2PolygonShape* get_shape() {
        return reinterpret_cast<b2PolygonShape*>(shape.get());
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(rect, states);
    }

private:
    float width;
    float height;

    sf::RectangleShape rect;
};