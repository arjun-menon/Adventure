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
            //ab = new PlaceholderEntityAABB(Pt(50,50), Dim(brick->w(), brick->h()));
            loaded = true;
        }
        else {
            //Sys()->drawTex(*brick, Pt(50, 50), false);
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
