#include "BaseFramework.hpp"
#include <SFML/Graphics.hpp>

class SystemImpl : public System
{
    static SystemImpl* singleton;

    friend int main(int argc, char *argv[]);
    friend System* getSystem();

    WindowProperties windowProperties;
    Entity* master;
    sf::RenderWindow* renderWindow;

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
            // Default windows height & width:
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
        master = getMaster(windowProperties);
        renderWindow = createRenderWindow();

        renderWindow->clear();
        renderWindow->setFramerateLimit( 60 );

        while( renderWindow->isOpen() ) {
            sf::Event event;
            while( renderWindow->pollEvent(event) ) {
                if( event.type == sf::Event::Closed )
                    renderWindow->close();
            }

            renderWindow->clear();
            master->step();
            renderWindow->display();
        }

        delete renderWindow;
        delete master;
    }

public:
    SystemImpl() : master(nullptr), renderWindow(nullptr) {}

    ~SystemImpl() {
        delete renderWindow;
        delete master;
    }

    void setMouseCursorVisibility(bool visibility) {
        renderWindow->setMouseCursorVisible( visibility );
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
