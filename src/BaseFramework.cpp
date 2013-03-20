/*
 * BaseFramework.cpp
 */

#include <chrono>
#include <random>

#include <SFML/Graphics.hpp>

#include "BaseFramework.hpp"

class SystemImpl : public System
{
    static SystemImpl* singleton;

    friend int main(int argc, char *argv[]);
    friend System* Sys();

    class TexImpl : public Tex
    {
        sf::Image image;
    public:
        sf::Texture tex;
        Dim sz;

        TexImpl(string file) {
            if( !image.loadFromFile(file) ) {
                throw invalid_argument("Unable to open file: " + file);
            }
            if( !tex.loadFromImage(image) ) {
                throw runtime_error("Loading image from texture " + file + " failed.");
            }
            sf::Vector2u size = tex.getSize();
            sz.w = static_cast<float>( size.x );
            sz.h = static_cast<float>( size.y );
        }

        Dim getDim() {
            return sz;
        }
    };

    WindowProperties windowProperties;
    unique_ptr<Entity> master;
    unique_ptr<sf::RenderWindow> renderWindow;

    std::minstd_rand0 rng;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullscreen = true;
        }
        else if( argc == 4 && !strcmp(argv[1], "-r") ) {
            windowProperties.dim= Dim( static_cast<float>( atoi(argv[2]) ),
                                       static_cast<float>( atoi(argv[3]) ) );
        }
        else {
            // Default window height & width:
            windowProperties.dim = Dim(1024, 600);
            windowProperties.fullscreen = false;
        }
    }

    sf::RenderWindow* createRenderWindow()
    {
        if(windowProperties.fullscreen) {
            sf::VideoMode best_mode = sf::VideoMode::getFullscreenModes()[0];
            windowProperties.dim.w = static_cast<float>( best_mode.width );
            windowProperties.dim.h = static_cast<float>( best_mode.height );
            return new sf::RenderWindow(best_mode, windowProperties.title, sf::Style::Fullscreen);
        }
        else
            return new sf::RenderWindow(
                sf::VideoMode( static_cast<int>( windowProperties.dim.w ), static_cast<int>( windowProperties.dim.h ), 32),
                    windowProperties.title );
    }

    void main(int argc, char *argv[])
    {
        handleCmdlineArgs(argc, argv);
        master = unique_ptr<Entity>( getMaster(windowProperties) );

        renderWindow = unique_ptr<sf::RenderWindow>( createRenderWindow() );
        renderWindow->clear();
        renderWindow->setFramerateLimit( 60 );

        while( renderWindow->isOpen() ) {
            sf::Event event;
            while( renderWindow->pollEvent(event) ) {
                if( event.type == sf::Event::Closed )
                    renderWindow->close();
            }

            renderWindow->clear();
            try {
                master->step();
                renderWindow->display();
            }
            catch(exception &e) {
                cerr<<e.what()<<endl;
                renderWindow->close();
            }
        }
    }

public:
    SystemImpl() : master(nullptr), renderWindow(nullptr),
                   rng( std::chrono::system_clock::now().time_since_epoch().count() ) {
    }

    ~SystemImpl() {
    }

    shared_ptr<Tex> loadTexFromImage(string file) {
        return shared_ptr<Tex>( new TexImpl(file) );
    }

    void setMouseCursorVisibility(bool visibility) {
        renderWindow->setMouseCursorVisible( visibility );
    }

    void drawImage(Tex &tex, Pt pos, bool flip=false, float angle=0.0f) {
        sf::Sprite sprite( dynamic_cast<TexImpl&>(tex).tex );
        sprite.setPosition( pos.x + (flip ? tex.w() : 0) , windowProperties.dim.h - tex.h() - pos.y );
        sprite.setRotation(angle);
        if(flip) {
            sprite.setScale(-1, 1);
        }
        renderWindow->draw(sprite);
    }

    void drawText(string line, Pt pos, Color color=Color(), float fontSize=15.0f) {
        sf::Text textToDraw(line, sf::Font::getDefaultFont(), static_cast<unsigned int>(fontSize));
        textToDraw.setColor(sf::Color(color.r, color.g, color.b, color.a));
        textToDraw.setPosition( static_cast<float>( pos.x ), static_cast<float>( pos.y ) );
        renderWindow->draw(textToDraw);
    }

    void drawBox(Pt pos, Dim size, Color fillColor, Color outlineColor, float outlineThickness) {
        sf::RectangleShape rectangle;
        rectangle.setPosition ( sf::Vector2f( pos.x,  windowProperties.dim.h - pos.y) );
        rectangle.setSize     ( sf::Vector2f(size.w, -size.h) );
        rectangle.setFillColor( sf::Color( fillColor.r, fillColor.g, fillColor.b, fillColor.a) );
        rectangle.setOutlineThickness( outlineThickness );
        rectangle.setOutlineColor( sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a) );
        renderWindow->draw(rectangle);
    }

    void exit() {
        renderWindow->close();
    }

    unsigned int random() {
        return static_cast<unsigned int>( rng() );
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
