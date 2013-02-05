/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

class Master : public Entity
{
    System &sys;
    bool loaded;
    Dim windowDim;

    shared_ptr<Tex> brick;

public:
    Master(WindowProperties &windowProperties) :
        sys(*getSystem()),
        loaded(false),
        windowDim(windowProperties.dim)
    {
        windowProperties.title = "Adventure";
    }
    void step() {
        if( !loaded ) {
            brick = sys.loadTexFromImage("brick.bmp");
            loaded = true;
        }
        else {
            sys.drawTex(*brick, Pt(10, 10), false);
            sys.drawText("hello", Pt(0,0));
        }
    }
    ~Master() {
    }
};

Entity* getMaster(WindowProperties &windowProperties) {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master(windowProperties);
    return master;
}
