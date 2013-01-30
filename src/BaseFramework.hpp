/*
 * BaseFramework.hpp
 */

#ifndef BASEFRAMEWORK_HPP_
#define BASEFRAMEWORK_HPP_

#include <memory>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
using namespace std;

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

class Tex
{
public:
    virtual Dim getDim() = 0;
    inline int w() { return getDim().w; }
    inline int h() { return getDim().h; }
    virtual ~Tex() {}
};

class System
{
public:
    virtual shared_ptr<Tex> loadTexFromImage(string file) = 0;
    virtual void setMouseCursorVisibility(bool visibility) = 0;
    virtual void drawTex(Tex &tex, Pt pos, bool flip=false, float angle=0.0f) = 0;
    virtual ~System() {}
};

Entity* getMaster(WindowProperties &windowProperties);

System* getSystem();

#endif /* BASEFRAMEWORK_HPP_ */
