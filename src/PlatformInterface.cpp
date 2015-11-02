/*
 * PlatformInterface.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"

#include <chrono>
#include <random>

#include <SDL2/SDL.h>

class SystemImpl : public System
{
public:
    SystemImpl() : gameMain(nullptr), eventCallbacks(nullptr), window(nullptr), renderer(nullptr), isRunning(false),
                   rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) { }

    ~SystemImpl() { }

    shared_ptr<Tex> loadTex(const string file) {
        // TODO
        return nullptr;
//        if( fileTexMap.find(file) == fileTexMap.end() ) {
//            // Load the texture
//            sf::Image image;
//            if( !image.loadFromFile(file) )
//                throw invalid_argument("Unable to open file: " + file);
//
//            sf::Texture* tex = new sf::Texture;
//            if( !tex->loadFromImage(image) )
//                throw runtime_error("Loading image from texture " + file + " failed.");
//
//            fileTexMap[file] = tex;
//        }
//
//        sf::Texture* tex = fileTexMap[file];
//
//        sf::Vector2u sz = tex->getSize();
//        xy size( static_cast<float>( sz.x ),
//                 static_cast<float>( sz.y ) );
//
//        return shared_ptr<Tex>( new TexImpl(tex, size) );
    }

    void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip, const double angle) {
//        // TODO
//        sf::Sprite sprite( *(dynamic_cast<const TexImpl&>(tex).tex) );
//        sprite.setPosition( pos.x + (horizontalFlip ? tex.getSize().x : 0) ,
//                            windowProperties.size.y - tex.getSize().y - pos.y );
//        sprite.setRotation(angle);
//        if(horizontalFlip) {
//            sprite.setScale(-1, 1);
//        }
//        window->draw(sprite);
    }

    void drawText(const string line, const xy pos, const Color color=Color(), const float fontSize=15.0f) {
//        // TODO
//        sf::Text textToDraw(line, defaultFont, static_cast<unsigned int>(fontSize));
//        textToDraw.setColor(sf::Color(color.r, color.g, color.b, color.a));
//        textToDraw.setPosition( static_cast<float>( pos.x ), static_cast<float>( pos.y ) );
//        window->draw(textToDraw);
    }

    void drawBox(const xy pos, const xy size, const Color fillColor, const Color outlineColor, const float outlineThickness) {
        SDL_Rect outlineRect = {pos.x, pos.y, size.x, size.y};
        SDL_Rect fillRect = getInnerRect(outlineRect);

        // Convert to cartesian coordinates
        outlineRect = convertToCartesianCoordinates(outlineRect);
        fillRect = convertToCartesianCoordinates(fillRect);

        SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        SDL_RenderDrawRect(renderer, &outlineRect);

        SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_RenderFillRect(renderer, &fillRect);

        // Reset drawing color to opaque black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }

    SDL_Rect convertToCartesianCoordinates(const SDL_Rect &rect) {
        SDL_Rect cartesianRect = {
                rect.x,
                windowProperties.size.y - rect.y,
                rect.w,
                - rect.h
        };
        return cartesianRect;
    }

    static SDL_Rect getInnerRect(const SDL_Rect &rect) {
        SDL_Rect innerRect = {
                rect.x + 1,
                rect.y,
                rect.w - 2,
                rect.h
            };
        return innerRect;
    }

    void setEventCallbacks(InputCallbacks *callbacks) {
        eventCallbacks = callbacks;
    }

    unsigned int random() {
        return static_cast<unsigned int>( rng() );
    }

    void exit() {
        isRunning = false;
    }

private:
    friend int main(int argc, char *argv[]);
    friend System* Sys();

    static SystemImpl* singleton;

    map<const string, SDL_Texture*> fileTexMap;

    class TexImpl : public Tex
    {
    public:
        SDL_Texture* tex;

        TexImpl(SDL_Texture* tex, xy size) :
            tex(tex), size(size) {}

        const xy getSize() const {
            return size;
        }

    private:
        xy size;
    };

    unique_ptr<GameMain> gameMain;
    InputCallbacks *eventCallbacks;

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    std::minstd_rand0 rng;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullscreen = true;
        }
        else if( argc == 4 && !strcmp(argv[1], "-r") ) {
            windowProperties.size = xy(atoi(argv[2]), atoi(argv[3]) );
        }
    }

    void initSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw runtime_error(string("SDL_Init Error: ") + SDL_GetError());
        }
    }

    void createWindow() {
        if (windowProperties.fullscreen) {
            throw invalid_argument("Unimplemented");
        }
        else {
            if ((window = SDL_CreateWindow(windowProperties.title.c_str(),
                                           160, 100, // todo: window position
                                           windowProperties.size.x,
                                           windowProperties.size.y,
                                           SDL_WINDOW_SHOWN)) == nullptr) {
                throw invalid_argument(string("SDL_CreateWindow Error: ") + SDL_GetError());
            }
        }
    }

    void createRenderer() {
        renderer = SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr){
            SDL_DestroyWindow(window);
            throw invalid_argument(string("SDL_CreateRenderer Error: ") + SDL_GetError());
        }
    }

//    string getFPS(int currentTime)
//    {
//        static const int updateFrequency = 10;
//        static int lastUpdate = 0,
//                lastFrameTime = 0,
//                duration = 0, fps = 0;
//        duration = currentTime - lastFrameTime;
//        if(duration && !lastUpdate) {
//            fps = 1000 / duration;
//            lastUpdate = updateFrequency;
//        }
//        lastFrameTime = currentTime;
//        lastUpdate--;
//
//        stringstream s;
//        s<<fps;
//        return s.str();
//    }

    void dispatchEvent(SDL_Event sdlEvent)
    {
        if (eventCallbacks == nullptr) {
            return;
        }
        else if (sdlEvent.type == SDL_KEYDOWN) {
            const SDL_Keycode key = sdlEvent.key.keysym.sym;

            if(key == SDLK_ESCAPE) {
                eventCallbacks->escKey();
            }
            else if(key == SDLK_UP || key == SDLK_w) {
                eventCallbacks->upKey();
            }
            else if(key == SDLK_LEFT || key == SDLK_a) {
                eventCallbacks->leftKey();
            }
            else if(key == SDLK_RIGHT || key == SDLK_d) {
                eventCallbacks->rightKey();
            }
            else if(key == SDLK_DOWN || key == SDLK_s) {
                eventCallbacks->downKey();
            }
        }
    }

    int platformMain(int argc, char *argv[])
    {
        windowProperties = GameMain::defaultWindowProperties();
        handleCmdlineArgs(argc, argv);

        /*
         * Platform Setup
         */
        try {
            initSDL();
            createWindow();
            createRenderer();
        }
        catch(exception &e) {
            cerr<<"Exception caught: "<<e.what()<<endl;
            SDL_Quit();
            return EXIT_FAILURE;
        }

        /*
         * Game Loop
         */
        try
        {
            gameMain = unique_ptr<GameMain>( GameMain::getSingleton() );
            isRunning = true;

            while(isRunning)
            {
                SDL_Event sdlEvent;
                while( SDL_PollEvent(&sdlEvent) )
                {
                    if(sdlEvent.type == SDL_QUIT ) {
                        isRunning = false; break;
                    }
                    else {
                        dispatchEvent(sdlEvent);
                    }
                }

                SDL_RenderClear(renderer);

                gameMain->step();

                SDL_RenderPresent(renderer);
            }

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_SUCCESS;
        }
        catch(exception &e)
        {
            cerr<<"Exception caught: "<<e.what()<<endl;

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }
};

SystemImpl* SystemImpl::singleton = nullptr;

System* Sys() {
    return SystemImpl::singleton;
}

int main(int argc, char *argv[]) {
    SystemImpl *sys = SystemImpl::singleton = new SystemImpl();
    int retCode = sys->platformMain(argc, argv);
    delete SystemImpl::singleton;
    return retCode;
}
