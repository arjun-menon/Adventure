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

    virtual void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip=false, const double angle=0.0f) = 0;
    virtual void drawText(const string line, const xy pos, const Color color=Color(), const float fontSize=15.0f) = 0;
    virtual void drawBox(const xy pos, const xy size, const Color fillColor=Color(255,255,255,0), const Color outlineColor=Color(), const float outlineThickness=1.0f) = 0;

    virtual void setEventCallbacks(InputCallbacks *callbacks) = 0;

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
