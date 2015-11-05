/*
 * GameMain.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"
#include "EntityMap.hpp"
#include "PhysicsMap.hpp"
#include "SideScrollingView.hpp"
#include <SDL2/SDL_keycode.h>

class StaticColoredBox : public Entity
{
public:
    StaticColoredBox(xy pos, xy sz) : Entity(new ColoredBox(sz), pos) {}
};

DynamicEntityCharacteristics playerDynamicChars(
        1, // groundFriction
        1, // gravityFactor
        12, // maxHorizontalSpeed
        2, // horizontalWalkStep
        25 // jumpStep
    );

class DynamicColoredBox : public DynamicEntity
{
public:
    DynamicColoredBox(xy pos, xy sz, const DynamicEntityCharacteristics &dynamicChars) :
        DynamicEntity(new ColoredBox(sz), pos, dynamicChars) {}
};

class Steppable
{
public:
    virtual void step() = 0;
    virtual ~Steppable() {}
};

class TestBed : public Steppable
{
    SideScrollingView sideScrollingView;
    PhysicsMap physicsMap;
    vector<StaticColoredBox> boxes;
    DynamicEntity *player;

public:
    TestBed() : physicsMap(sys->getWindowProperties().size)
    {
        boxes.push_back(StaticColoredBox( xy(0, 1) , xy(200, 100) ));
        boxes.push_back(StaticColoredBox( xy(202, 0) , xy(500, 3) ));
        boxes.push_back(StaticColoredBox( xy(270, 140) , xy(100, 20) ));
        boxes.push_back(StaticColoredBox( xy(300, 290) , xy(100, 100) ));
        boxes.push_back(StaticColoredBox( xy(500, 400) , xy(400, 100) ));
        player = new DynamicColoredBox( xy(100, 170) , xy(25, 25), playerDynamicChars );

        set<Entity *> collidingEntities;
        for(auto &box : boxes)
            physicsMap.place(&box, collidingEntities);

        physicsMap.place(player, collidingEntities);

        sideScrollingView.physicsMap = &physicsMap;
        sideScrollingView.player = player;
    }

    void step()
    {
        handleInput();

        physicsMap.performPhysics();

        sideScrollingView.render();

        stringstream ss;
        std::function<void (Entity*)> sscat = [&ss](Entity *e) {
            Rect rect = e->getRect();
            xy exterm = rect.pos + rect.size;
            ss<<" has "<<rect<<" and upper-right corner at "<<exterm<<"\n";
        };

        for(unsigned int i = 0; i < boxes.size(); i++) {
            ss<<"Box["<<i<<"]";
            sscat(&boxes[i]);
        }
        ss<<"Player";
        sscat(player);
        sys->drawText(ss.str(), xy(10,10));
    }

    void handleInput()
    {
        if(sys->isPressed(SDLK_ESCAPE)) {
            sys->exit();
        }
        if(sys->isPressed(SDLK_UP) || sys->isPressed(SDLK_w)) {
            physicsMap.jump(player);
        }
        if(sys->isPressed(SDLK_LEFT) || sys->isPressed(SDLK_a)) {
            physicsMap.walkLeft(player);
        }
        if(sys->isPressed(SDLK_RIGHT) || sys->isPressed(SDLK_d)) {
            physicsMap.walkRight(player);
        }
    }
};

GameMain* GameMain::singleton = nullptr;

class GameMainImpl : public GameMain
{
    unique_ptr<Steppable> whatever;

public:
    GameMainImpl()
    {
        whatever = unique_ptr<Steppable>( new TestBed() );
    }

    void step() {
        whatever->step();
    }

    ~GameMainImpl() {
    }
};

GameMain* GameMain::getSingleton() {
    if( GameMain::singleton == nullptr )
        GameMain::singleton = new GameMainImpl();
    return GameMain::singleton;
}
