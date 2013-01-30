/*
 * BaseFramework.hpp
 */

#ifndef BASEFRAMEWORK_HPP_
#define BASEFRAMEWORK_HPP_

#include <memory>
#include <iostream>
#include <cstring>
#include <string>

#include "GeometricPrimitives.hpp"

class Entity
{
public:
    virtual void step() {};
    virtual ~Entity() {}
};

struct WindowProperties
{
    Dim dim;
    bool fullscreen;
    std::string title;
};

Entity* getMaster(WindowProperties &windowProperties);

class System
{
public:
    virtual void setMouseCursorVisibility(bool visibility) = 0;
    virtual ~System() {}
};

System* getSystem();

#endif /* BASEFRAMEWORK_HPP_ */
