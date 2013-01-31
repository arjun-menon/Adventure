/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

class Master : public Entity
{
    System &sys;

    bool loaded;
    shared_ptr<Tex> brick;

public:
    Master(WindowProperties &windowProperties) :
        sys(*getSystem()),
        loaded(false),
        brick(nullptr)
    {
        windowProperties.title = "Adventure";
    }
    void step() {
        if( !loaded ) {
            brick = sys.loadTexFromImage("brick.bmp");
            loaded = true;
        }
        else {
            static float angle = 0.0f;
            sys.drawTex(*brick, Pt(300, 300), false, angle);
            sys.drawText("hello", Pt(0,0));
            angle += 7.5f;
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
