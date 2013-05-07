/*
 * Main.cpp
 */

#include "BaseFramework.hpp"
#include "SideScrollingView.hpp"

class StaticColoredBox : public Entity
{
public:
    StaticColoredBox(xy pos, xy sz) : Entity(new ColoredBox(sz), pos) {}
};

DynamicEntityCharacteristics dChar1(0.1f, 0.2f, 5.0f);
DynamicEntityCharacteristics dChar2(0.1f, 0.2f, 5.0f, 0.3f, 11.0f);

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
    SideScrollingView ssv;
    PhysicsMap physicsMap;
    Entity *a, *b, *c;

public:
    TestBed() : physicsMap( Sys()->getWindowProperties().size, 32 )
    {
        a = new StaticColoredBox( xy(10, 10) , xy(200, 100) );
        b = new StaticColoredBox( xy(220, 210) , xy(100, 100) );
        b = new DynamicColoredBox( xy(220, 210) , xy(100, 100), dChar1 );
        c = new DynamicColoredBox( xy(300, 170) , xy(10, 10), dChar1 );

        set<Entity *> collidingEntities;
        physicsMap.place(a, collidingEntities);
        physicsMap.place(b, collidingEntities);
        physicsMap.place(c, collidingEntities);

        ssv.physicsMap = &physicsMap;
    }

    void step()
    {
        physicsMap.performPhysics();

        static set<Entity *> collidingEntities;
        collidingEntities.clear();
        physicsMap.entityMap.moveBy(b, xy(5, 0), collidingEntities);

        ssv.render();
    }
};

class OldGameMap : public Steppable, public InputCallbacks
{
    DynamicEntity *pivot;
    unique_ptr<PhysicsMap> physicsMap;
    SideScrollingView ssv;

public:
    OldGameMap() : pivot(nullptr)
    {
        // get images:
        map<const string, shared_ptr<Tex>> images;

        ifstream img_files("images.txt");

        if(img_files.fail())
            throw invalid_argument("Unable to open image list file.");

        string s;

        while(!img_files.eof())
        {
            string image_name, file_name;

            getline(img_files, image_name, ',');
            getline(img_files, file_name);

            images[image_name] = Sys()->loadTex(file_name);
        }

        img_files.close();

        // load level

        ifstream level_file("level.txt");

        if(level_file.fail())
            throw invalid_argument("Unable to open level file.");

        xy size;

        string d;
        getline(level_file, d);
        size.x = atoi(d.c_str());
        getline(level_file, d);
        size.y = atoi(d.c_str());

        physicsMap = move( unique_ptr<PhysicsMap>( new PhysicsMap(size, 32) ) );

        set<Entity *> collidingEntities;

        while(!level_file.eof())
        {
            string s;
            xy p;

            getline(level_file, s, ',');

            if(level_file.eof())
                break;

            getline(level_file, d, ',');
            p.x = atoi(d.c_str());
            getline(level_file, d);
            p.y = atoi(d.c_str());

            DrawableAABB *d = new SimpleImage( images.at(s) );

            Entity *e = nullptr;

            if(s == "Player") {
                pivot = new DynamicEntity(d, p, dChar2);
                e = pivot;
            }
            else
                e = new Entity(d, p);

            if( !physicsMap->place(e, collidingEntities) ) {
                cerr<<"Collision"<<endl;
                break;
            }
        }

        level_file.close();

        // setup scv
        ssv.physicsMap = &*physicsMap;
        ssv.pivot = pivot;
    }

    void upKey() { physicsMap->jump(pivot); }
    void leftKey() { physicsMap->walkLeft(pivot); }
    void rightKey() { physicsMap->walkRight(pivot); }

    void step() {
        Sys()->getInput(this);
        physicsMap->performPhysics();
        ssv.render();
    }
};

GameMain* GameMain::singleton = nullptr;

class GameMainImpl : public GameMain
{
    bool loaded;

    unique_ptr<Steppable> whatever;

public:
    GameMainImpl() : loaded(false)
    {
        //whatever = unique_ptr<Steppable>( new TestBed() );
        whatever = unique_ptr<Steppable>( new OldGameMap() );
    }

    void step() {
        whatever->step();
    }

    ~GameMainImpl() {
    }
};

WindowProperties GameMain::defaultWindowProperties()
{
    WindowProperties windowProperties;

    // Default window height & width:
    windowProperties.size = xy(1024, 600);
    windowProperties.fullscreen = false;

    // Title
    windowProperties.title = "Adventure";

    return windowProperties;
}

GameMain* GameMain::getSingleton() {
    if( GameMain::singleton == nullptr )
        GameMain::singleton = new GameMainImpl();
    return GameMain::singleton;
}
