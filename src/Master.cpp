/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

class Splash : public Entity
{
    //
};

class Master : public Entity
{
    bool loaded;
    Dim windowDim;

    shared_ptr<Tex> brick;
    PlaceholderEntityAABB *b;

public:
    Master(WindowProperties &windowProperties) :
        loaded(false),
        windowDim(windowProperties.dim),
        b(nullptr)
    {
        windowProperties.title = "Adventure";
    }
    void step() {
        if( !loaded ) {
            brick = Sys()->loadTexFromImage("brick.bmp");
            b = new PlaceholderEntityAABB( Pt(50,50), Dim(10,10) );
            loaded = true;
        }
        else {
            Sys()->drawImage(*brick, Pt(0, 0), false);
            Sys()->drawText("hello", Pt(0,0));

            b->step();
        }
    }
    ~Master() {
        delete b;
    }
};

Entity* getMaster(WindowProperties &windowProperties) {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master(windowProperties);
    return master;
}
