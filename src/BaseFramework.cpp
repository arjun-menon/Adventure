#include "BaseFramework.hpp"
#include <SFML/Graphics.hpp>

class TexImpl : public Tex
{
    sf::Image image;
public:
    sf::Texture tex;
    Dim sz;

    TexImpl(string file) {
        if( !image.loadFromFile(file) ) {
            throw invalid_argument(string("Unable to open file: ") + file);
        }
        if( !tex.loadFromImage(image) ) {
            throw runtime_error("Loading image from texture " + file + " failed.");
        }
        sf::Vector2u size = tex.getSize();
        sz.w = size.x; sz.h = size.y;
    }

    Dim getDim() {
        return sz;
    }
};

class SystemImpl : public System
{
    static SystemImpl* singleton;

    friend int main(int argc, char *argv[]);
    friend System* getSystem();

    WindowProperties windowProperties;
    unique_ptr<Entity> master;
    unique_ptr<sf::RenderWindow> renderWindow;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullscreen = true;
        }
        else if( argc == 4 && !strcmp(argv[1], "-r") ) {
            windowProperties.dim= Dim( atoi(argv[2]), atoi(argv[3]) );
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
            windowProperties.dim.w = best_mode.width;
            windowProperties.dim.h = best_mode.height;
            return new sf::RenderWindow(best_mode, windowProperties.title, sf::Style::Fullscreen);
        }
        else
            return new sf::RenderWindow(sf::VideoMode(windowProperties.dim.w, windowProperties.dim.h, 32), windowProperties.title);
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
    SystemImpl() : master(nullptr), renderWindow(nullptr) {}
    ~SystemImpl() {}

    shared_ptr<Tex> loadTexFromImage(string file) {
        return shared_ptr<Tex>( new TexImpl(file) );
    }

    void setMouseCursorVisibility(bool visibility) {
        renderWindow->setMouseCursorVisible( visibility );
    }

    void drawTex(Tex &tex, Pt pos, bool flip=false, float angle=0.0f) {
        sf::Sprite sprite( dynamic_cast<TexImpl&>(tex).tex );
        sprite.setPosition( sf::Vector2<float>(
                static_cast<float>( pos.x + (flip ? tex.w() : 0) ),
                static_cast<float>( windowProperties.dim.h - tex.h() - pos.y ) ) );
        sprite.setRotation(angle);
        if(flip) {
            sprite.setScale(-1, 1);
        }
        renderWindow->draw(sprite);
    }

    void exit() {
        renderWindow->close();
    }
};

SystemImpl* SystemImpl::singleton = nullptr;

System* getSystem() {
    return SystemImpl::singleton;
}

int main(int argc, char *argv[]) {
    SystemImpl *sys = SystemImpl::singleton = new SystemImpl();
    sys->main(argc, argv);
    delete SystemImpl::singleton;
    return 0;
}
