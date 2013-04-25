/*
 * Main.cpp
 */

#include "BaseFramework.hpp"
#include "SideScrollingMap.hpp"

class StaticColoredBox : public Entity
{
public:
    StaticColoredBox(xy pos, xy sz) : Entity(new ColoredBox(sz), pos) {}
};

class DynamicColoredBox : public DynamicEntity
{
public:
    DynamicColoredBox(xy pos, xy sz, float friction, float gravity) :
        DynamicEntity(new ColoredBox(sz), pos, friction, gravity) {}
};

class Steppable
{
public:
    virtual void step() = 0;
    virtual ~Steppable() {}
};

class TestBed : public Steppable
{
    SideScrollingMap m;
    Entity *a, *b, *c;

public:
    TestBed() : m( Sys()->getWindowProperties().size, 32 ) {
        a = new StaticColoredBox( xy(10, 10) , xy(200, 100) );
        b = new StaticColoredBox( xy(220, 210) , xy(100, 100) );
        b = new DynamicColoredBox( xy(220, 210) , xy(100, 100), 0.1f, 0.2f );
        c = new DynamicColoredBox( xy(300, 170) , xy(10, 10), 0.1f, 0.05f );

        set<Entity *> collidingEntities;
        m.place(a, collidingEntities);
        m.place(b, collidingEntities);
        m.place(c, collidingEntities);

        //dynamic_cast<DynamicEntityTrait&>(*c).velocity.x = -7.0f;
    }

    void step() {
        m.performPhysics();

        set<Entity *> collidingEntities;
        m.entityMap.moveBy(b, xy(5, 0), collidingEntities);

        m.step();
    }
};

class OldGameMap : public Steppable
{
    unique_ptr<SideScrollingMap> m;

public:
    OldGameMap()
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

        m = move( unique_ptr<SideScrollingMap>( new SideScrollingMap(size, 32) ) );

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

            Entity *e = new Entity(d, p);

            if( !m->place(e, collidingEntities) ) {
                cerr<<"Collision"<<endl;
                break;
            }
        }

        level_file.close();
    }

    void step() {
        m->step();
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
        whatever = unique_ptr<Steppable>( new TestBed() );
        //whatever = unique_ptr<Steppable>( new OldGameMap() );
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
