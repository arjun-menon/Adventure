/*
 * PlatformInterface.hpp
 */

#ifndef PLATFORMINTERFACE_HPP_
#define PLATFORMINTERFACE_HPP_

struct WindowProperties
{
    xy size;
    bool fullscreen;
    std::string title;
};

class InputCallbacks
{
public:
    virtual void escKey() {}
    virtual void upKey() {}
    virtual void leftKey() {}
    virtual void rightKey() {}
    virtual void downKey() {}

    virtual ~InputCallbacks() {}
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

#endif /* PLATFORMINTERFACE_HPP_ */
