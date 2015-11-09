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
    SystemImpl() : game(nullptr), window(nullptr), renderer(nullptr), keepRunning(false),
                   rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) { }

    ~SystemImpl() { }

    shared_ptr<Tex> loadTex(const string imageFilePath) {
        if(texMap.find(imageFilePath) == texMap.end() ) {
            // Load SDL Surface
            SDL_Surface *surface = SDL_LoadBMP(imageFilePath.c_str());
            if (surface == nullptr)
                throw runtime_error(string("Unable to load file: ") + SDL_GetError());

            xy size(surface->w, surface->h);

            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if(tex == nullptr)
                throw runtime_error("Loading texture from image " + imageFilePath + " failed.");

            texMap[imageFilePath] = shared_ptr<Tex>(new TexImpl(tex, size));
        }

        return texMap[imageFilePath];
    }

    void drawImage(const Tex &tex, const xy pos, const bool horizontalFlip, const double angle) {
        SDL_Rect dstRect = { pos.x, pos.y, tex.getSize().x, tex.getSize().y };
        SDL_Texture *texture = dynamic_cast<const TexImpl&>(tex).texture;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (horizontalFlip)
            flip = SDL_FLIP_HORIZONTAL;

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
        return (SDL_Rect) {
            rect.x,
            windowProperties.size.y - rect.y,
            rect.w,
            - rect.h
        };
    }

    static SDL_Rect getInnerRect(const SDL_Rect &rect) {
        return (SDL_Rect) {
            rect.x + 1,
            rect.y,
            rect.w - 2,
            rect.h
        };
    }

    bool isPressed(int keyCode) {  // keyCode is expecte to be an SDL_Keycode
        return pressedKeys.find(keyCode) != pressedKeys.end();
    }

    unsigned int random(unsigned int range) {
        return rng() % range;
    }

    void exit() {
        keepRunning = false;
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

    unique_ptr<Game> game;
    map<const string, shared_ptr<Tex>> texMap;
    set<SDL_Keycode> pressedKeys;
    set<SDL_Keycode> toUnpress;

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool keepRunning;

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
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr) {
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

    void processKeysToUnpress() {
        for(SDL_Keycode keyToUnpress : toUnpress)
            if(isPressed(keyToUnpress))
                pressedKeys.erase(keyToUnpress);

        toUnpress.clear();
    }

    void handleEvents()
    {
        SDL_Event event;
        set<SDL_Keycode> firstPress;

        processKeysToUnpress();

        while(SDL_PollEvent(&event))
        {
            const SDL_Keycode keyCode = event.key.keysym.sym;

            if(event.type == SDL_QUIT) {
                keepRunning = false;
                break;
            }
            else if(event.type == SDL_KEYDOWN && !isPressed(keyCode)) {
                pressedKeys.insert(keyCode);
                firstPress.insert(keyCode);
            }
            else if(event.type == SDL_KEYUP && isPressed(keyCode)) {
                if(firstPress.find(keyCode) == firstPress.end()) {
                    pressedKeys.erase(keyCode);
                }
                else {
                    // we got a KEYDOWN for this key during this frame, so do
                    // not un-press it now, but schedule it to be un-pressed.
                    toUnpress.insert(keyCode);
                }
            }
        }
    }

    int platformMain(int argc, char *argv[])
    {
        handleCmdlineArgs(argc, argv);

        /*
         * Game Setup
         */
        try {
            game = Game::setup();
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
            keepRunning = true;

            while(keepRunning)
            {
                handleEvents();

                SDL_RenderClear(renderer);

                game->step();

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
    return (WindowProperties) {
            xy(160, 100), // default window position
            xy(800, 600),  // default window height & width
            false, // full screen
            "Adventure" // title
    };
}

System* sys;

int main(int argc, char *argv[]) {
    SystemImpl *sysImpl = new SystemImpl();
    sys = sysImpl;

    int retCode = sysImpl->platformMain(argc, argv);

    delete sysImpl;
    return retCode;
}
