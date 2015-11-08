/*
 * PlatformInterface.hpp
 */

#ifndef PLATFORMINTERFACE_HPP_
#define PLATFORMINTERFACE_HPP_

struct WindowProperties
{
    WindowProperties(const xy &pos, const xy &size, bool fullscreen, const string &title) :
            pos(pos), size(size), fullScreen(fullscreen), title(title) {}
    xy pos;
    xy size;
    bool fullScreen;
    std::string title;
};

class System
{
protected:
    WindowProperties windowProperties = defaultWindowProperties();

public:
    static WindowProperties defaultWindowProperties();
    virtual void setWindowProperties(const WindowProperties& windowProperties) = 0;
    const WindowProperties& getWindowProperties() { return windowProperties; }

    virtual shared_ptr<Tex> loadTex(string imageFilePath) = 0;

    virtual void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip=false, const double angle=0.0f) = 0;
    virtual void drawText(const string line, const xy pos, const Color color=Color(), const float fontSize=15.0f) = 0;
    virtual void drawBox(const xy pos, const xy size, const Color fillColor=Color(255,255,255,0), const Color outlineColor=Color(), const float outlineThickness=1.0f) = 0;

    virtual bool isPressed(int keyCode) = 0;

    virtual unsigned int random(unsigned int range) = 0;
    virtual unsigned int random() = 0;
    virtual void exit() = 0;
    virtual ~System() {}
};

extern System* sys; // Concrete implementation of System initialized in main().

class GameMain
{
public:
    static GameMain* getSingleton();

    virtual void setup() {};
    virtual void step() = 0; // Called every frame.

    virtual ~GameMain() {}

private:
    static GameMain* singleton;
};

#endif /* PLATFORMINTERFACE_HPP_ */
