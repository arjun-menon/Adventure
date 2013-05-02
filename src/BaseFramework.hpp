/*
 * BaseFramework.hpp
 */

#ifndef BASEFRAMEWORK_HPP_
#define BASEFRAMEWORK_HPP_

#include <stdexcept>
#include <memory>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cstring>
#include <cmath>

using namespace std;

#include "Elements.hpp"

struct WindowProperties
{
    xy size;
    bool fullscreen;
    std::string title;
};

class System
{
protected:
    WindowProperties windowProperties;

public:
    const WindowProperties& getWindowProperties() { return windowProperties; }

    virtual shared_ptr<Tex> loadTex(string file) = 0;

    virtual void drawImage(const Tex &tex, xy pos, bool flip=false, float angle=0.0f) = 0;
    virtual void drawText(string line, xy pos, Color color=Color(), float fontSize=15.0f) = 0;
    virtual void drawBox(xy pos, xy size, Color fillColor=Color(255,255,255,0), Color outlineColor=Color(), float outlineThickness=1.0f) = 0;

    virtual void setMouseCursorVisibility(bool visibility) = 0;
    virtual void getInput(InputCallbacks *callbacks) = 0;

    virtual unsigned int random() = 0;
    virtual void exit() = 0;
    virtual ~System() {}
};

System* Sys(); // get pointer to concrete System singleton

class GameMain
{
public:
    static WindowProperties defaultWindowProperties();
    static GameMain* getSingleton();

    virtual void step() = 0; // Called every frame.

    virtual ~GameMain() {}

private:
    static GameMain* singleton;
};

#endif /* BASEFRAMEWORK_HPP_ */
