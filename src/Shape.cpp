#include "Shape.hpp"


Shape::Shape() :
    shape(nullptr)
{

}

b2Shape* Shape::get_shape() {
    return shape.get();
}