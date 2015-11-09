/*
 * PlatformInterface.hpp
 */

#ifndef PLATFORMINTERFACE_HPP_
#define PLATFORMINTERFACE_HPP_

class Game
{
public:
    static unique_ptr<Game> setup();
    virtual void step() = 0; // called every frame
    virtual ~Game() {}
};

struct WindowProperties
{
    xy pos, size;
    bool fullScreen;
    std::string title;
};

class System
{
public:
    static WindowProperties defaultWindowProperties();
    WindowProperties windowProperties = defaultWindowProperties();

    virtual shared_ptr<Tex> loadTex(string imageFilePath) = 0;

    virtual void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip=false, const double angle=0.0f) = 0;
    virtual void drawText(const string line, const xy pos, const Color color=Color(), const float fontSize=15.0f) = 0;
    virtual void drawBox(const xy pos, const xy size, const Color fillColor=Color(255,255,255,0), const Color outlineColor=Color(), const float outlineThickness=1.0f) = 0;

    virtual bool isPressed(int keyCode) = 0;

    virtual unsigned int random(unsigned int range) = 0;
    virtual void exit() = 0;
    virtual ~System() {}
};

extern System* sys; // Concrete implementation of System initialized in main().

#endif /* PLATFORMINTERFACE_HPP_ */
