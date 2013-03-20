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
#include "Entity.hpp"

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
    inline float w() { return getDim().w; }
    inline float h() { return getDim().h; }
    virtual ~Tex() {}
};

class System
{
public:
    virtual shared_ptr<Tex> loadTexFromImage(string file) = 0;
    virtual void setMouseCursorVisibility(bool visibility) = 0;
    virtual void drawImage(Tex &tex, Pt pos, bool flip=false, float angle=0.0f) = 0;
    virtual void drawText(string line, Pt pos, Color color=Color(), float fontSize=15.0f) = 0;
    virtual void drawBox(Pt pos, Dim size, Color fillColor=Color(255,255,255,0), Color outlineColor=Color(), float outlineThickness=1.0f) = 0;

    virtual unsigned int random() = 0;
    virtual ~System() {}
};

Entity* getMaster(WindowProperties &windowProperties);

System* Sys(); // get pointer to concrete System singleton

#endif /* BASEFRAMEWORK_HPP_ */
