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

    shared_ptr<Tex> loadTex(const string imageFilePath) {
        if(texMap.find(imageFilePath) == texMap.end() ) {
            // Load SDL Surface
            SDL_Surface *surface = SDL_LoadBMP(imageFilePath.c_str());
            if (surface == nullptr){
                throw runtime_error(string("Unable to load file: ") + SDL_GetError());
            }

            xy size(surface->w, surface->h);

            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if(tex == nullptr) {
                throw runtime_error("Loading texture from image " + imageFilePath + " failed.");
            }

            texMap[imageFilePath] = shared_ptr<Tex>(new TexImpl(tex, size));
        }

        return texMap[imageFilePath];
    }

    void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip, const double angle) {
        SDL_Rect dstRect = { pos.x, pos.y, tex.getSize().x, tex.getSize().y };
        SDL_Texture *texture = dynamic_cast<const TexImpl&>(tex).texture;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (horizontalFlip) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, flip);
    }

    void drawText(const string line, const xy pos, const Color color, const float fontSize) {
//        TODO
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

    class TexImpl : public Tex
    {
    public:
        SDL_Texture*texture;

        TexImpl(SDL_Texture* tex, xy size) :
                texture(tex), size(size) {}

        TexImpl(const TexImpl &) = default;

        const xy getSize() const {
            return size;
        }

        ~TexImpl() {
            SDL_DestroyTexture(texture);
        }

    private:
        xy size;
    };

    unique_ptr<GameMain> gameMain;
    map<const string, shared_ptr<Tex>> texMap;
    InputCallbacks *eventCallbacks;

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    std::minstd_rand0 rng;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullScreen = true;
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

    void setWindowProperties(const WindowProperties& windowProperties)
    {
        this->windowProperties = windowProperties;
    }

    void createWindow()
    {
        if (windowProperties.fullScreen) {
            throw invalid_argument("Unimplemented");
        }
        else {
            if ((window = SDL_CreateWindow(windowProperties.title.c_str(),
                                           windowProperties.pos.x,
                                           windowProperties.pos.y,
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

WindowProperties System::defaultWindowProperties() {
    return WindowProperties(
            xy(160, 100), // default window position
            xy(800, 600),  // default window height & width
            false, // full screen
            "Adventure" // title
    );
}

System* sys;

int main(int argc, char *argv[]) {
    SystemImpl *sysImpl = new SystemImpl();
    sys = sysImpl;

    int retCode = sysImpl->platformMain(argc, argv);

    delete sysImpl;
    return retCode;
}
