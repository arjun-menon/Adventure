/*
 * BaseFramework.cpp
 */

#include <chrono>
#include <random>

#include <SFML/Graphics.hpp>

#include "BaseFramework.hpp"

class SystemImpl : public System
{
public:
    SystemImpl() : gameMain(nullptr), renderWindow(nullptr),
        rng( std::chrono::system_clock::now().time_since_epoch().count() ) {
    }

    ~SystemImpl() {
    }

    shared_ptr<Tex> loadTex(const string file) {
        if( fileTexMap.find(file) == fileTexMap.end() ) {
            // Load the texture
            sf::Image image;
            if( !image.loadFromFile(file) )
                throw invalid_argument("Unable to open file: " + file);

            sf::Texture* tex = new sf::Texture;
            if( !tex->loadFromImage(image) )
                throw runtime_error("Loading image from texture " + file + " failed.");

            fileTexMap[file] = tex;
        }

        sf::Texture* tex = fileTexMap[file];

        sf::Vector2u sz = tex->getSize();
        xy size( static_cast<float>( sz.x ),
                 static_cast<float>( sz.y ) );

        return shared_ptr<Tex>( new TexImpl(tex, size) );
    }

    void drawImage(const Tex &tex, xy pos, bool flip=false, float angle=0.0f) {
        sf::Sprite sprite( *(dynamic_cast<const TexImpl&>(tex).tex) );
        sprite.setPosition( pos.x + (flip ? tex.getSize().x : 0) ,
                            windowProperties.size.y - tex.getSize().y - pos.y );
        sprite.setRotation(angle);
        if(flip) {
            sprite.setScale(-1, 1);
        }
        renderWindow->draw(sprite);
    }

    void drawText(string line, xy pos, Color color=Color(), float fontSize=15.0f) {
        sf::Text textToDraw(line, defaultFont, static_cast<unsigned int>(fontSize));
        textToDraw.setColor(sf::Color(color.r, color.g, color.b, color.a));
        textToDraw.setPosition( static_cast<float>( pos.x ), static_cast<float>( pos.y ) );
        renderWindow->draw(textToDraw);
    }

    void drawBox(xy pos, xy size, Color fillColor, Color outlineColor, float outlineThickness) {
        sf::RectangleShape rectangle;
        rectangle.setPosition ( sf::Vector2f( pos.x,  windowProperties.size.y - pos.y) );
        rectangle.setSize     ( sf::Vector2f( size.x, -size.y ) );
        rectangle.setFillColor( sf::Color( fillColor.r, fillColor.g, fillColor.b, fillColor.a) );
        rectangle.setOutlineThickness( outlineThickness );
        rectangle.setOutlineColor( sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a) );
        renderWindow->draw(rectangle);
    }

    void setMouseCursorVisibility(bool visibility) {
        renderWindow->setMouseCursorVisible( visibility );
    }

    virtual void getInput(InputCallbacks *callbacks) {
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
            callbacks->upKey();
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
            callbacks->leftKey();
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
            callbacks->rightKey();
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
            callbacks->downKey();
    }

    unsigned int random() {
        return static_cast<unsigned int>( rng() );
    }

    void exit() {
        renderWindow->close();
    }

private:
    friend int main(int argc, char *argv[]);
    friend System* Sys();

    static SystemImpl* singleton;

    map<const string, sf::Texture*> fileTexMap;

    class TexImpl : public Tex
    {
    public:
        sf::Texture* tex;

        TexImpl(sf::Texture* tex, xy size) :
            tex(tex), size(size) {}

        const xy getSize() const {
            return size;
        }

    private:
        xy size;
    };

    unique_ptr<GameMain> gameMain;
    unique_ptr<sf::RenderWindow> renderWindow;

    sf::Font defaultFont;

    std::minstd_rand0 rng;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullscreen = true;
        }
        else if( argc == 4 && !strcmp(argv[1], "-r") ) {
            windowProperties.size = xy( static_cast<float>( atoi(argv[2]) ),
                                        static_cast<float>( atoi(argv[3]) ) );
        }
    }

    sf::RenderWindow* createRenderWindow()
    {
        if(windowProperties.fullscreen) {
            sf::VideoMode best_mode = sf::VideoMode::getFullscreenModes()[0];
            windowProperties.size.x = static_cast<float>( best_mode.width );
            windowProperties.size.y = static_cast<float>( best_mode.height );
            sf::RenderWindow *renderWindow = new sf::RenderWindow(
            		best_mode, windowProperties.title, sf::Style::Fullscreen);
            renderWindow->setVerticalSyncEnabled(true);
            return renderWindow;
        }
        else
            return new sf::RenderWindow(
                sf::VideoMode( static_cast<int>( windowProperties.size.x ),
                               static_cast<int>( windowProperties.size.y ), 32),
                    windowProperties.title );
    }

    string getFPS(int currentTime)
    {
        static const int updateFrequency = 10;
        static int lastUpdate = 0,
                lastFrameTime = 0,
                duration = 0, fps = 0;
        duration = currentTime - lastFrameTime;
        if(duration && !lastUpdate) {
            fps = 1000 / duration;
            lastUpdate = updateFrequency;
        }
        lastFrameTime = currentTime;
        lastUpdate--;

        stringstream s;
        s<<fps;
        return s.str();
    }

    void main(int argc, char *argv[])
    {
        windowProperties = GameMain::defaultWindowProperties();

        handleCmdlineArgs(argc, argv);

        renderWindow = unique_ptr<sf::RenderWindow>( createRenderWindow() );
        renderWindow->clear();
        renderWindow->setFramerateLimit( 60 );

        // setup the default Font
        defaultFont.loadFromFile("assets/fonts/opensans-regular-webfont.ttf");

        try
        {
            gameMain = unique_ptr<GameMain>( GameMain::getSingleton() );

            sf::Clock clock;

            while( renderWindow->isOpen() )
            {
                sf::Event event;
                while( renderWindow->pollEvent(event) )
                {
                    if( event.type == sf::Event::Closed )
                        renderWindow->close();
                }

                renderWindow->clear();

                gameMain->step();

                Sys()->drawText(getFPS( clock.getElapsedTime().asMilliseconds() ),
                        xy(Sys()->getWindowProperties().size.x - 30, 10));

                renderWindow->display();
            }
        }
        catch(exception &e)
        {
            cerr<<"Exception caught: "<<e.what()<<endl;
            renderWindow->close();
            cerr<<"Terminating..."<<endl;
        }
    }
};

SystemImpl* SystemImpl::singleton = nullptr;

System* Sys() {
    return SystemImpl::singleton;
}

int main(int argc, char *argv[]) {
    SystemImpl *sys = SystemImpl::singleton = new SystemImpl();
    sys->main(argc, argv);
    delete SystemImpl::singleton;
    return 0;
}
