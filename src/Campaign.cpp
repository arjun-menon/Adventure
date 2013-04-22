/*
 * Campaign.cpp
 */

#include "Elements.hpp"
#include "PhysicsMap.hpp"

class DynamicPlaceholderEntityAABB : public PlaceholderEntityAABB, public DynamicEntityTrait
{
public:
    DynamicPlaceholderEntityAABB(Pt pos, Dim sz, float friction, float gravity) :
        PlaceholderEntityAABB(pos, sz),
        DynamicEntityTrait(friction, gravity) {}
};

class TestBed : public Entity
{
    PhysicsMap m;
    PlaceholderEntityAABB *a, *b, *c;

public:
    TestBed() : m( Sys()->getWindowProperties().dim, 32 ) {
        a = new PlaceholderEntityAABB( Pt(10, 10) , Dim(200, 100) );
        //b = new PlaceholderEntityAABB( Pt(220, 210) , Dim(100, 100) );
        b = new DynamicPlaceholderEntityAABB( Pt(220, 210) , Dim(100, 100), 0.1f, 0.2f );
        c = new DynamicPlaceholderEntityAABB( Pt(300, 170) , Dim(10, 10), 0.1f, 0.2f );
        //b = new PlaceholderEntityAABB( Pt(220, 210) , Dim(5, 5) );
        //c = new PlaceholderEntityAABB( Pt(225.1, 210) , Dim(3, 3) );

        set<EntityAABB *> collidingEntities;
        m.place(a, collidingEntities);
        m.place(b, collidingEntities);
        m.place(c, collidingEntities);

        //dynamic_cast<DynamicEntityTrait&>(*c).velocity.x = -7.0f;

        OptimizationMatrix optmat(Dim(1024, 600), 32);
    }

    void step() {
        m.step();

        set<EntityAABB *> collidingEntities;
        m.entityMap.moveBy(b, Pt(5, 0), collidingEntities);
    }
};


class OldGameMap : public Entity
{
    unique_ptr<PhysicsMap> m;

public:
    OldGameMap()
    {
        // get images:
        map<string, shared_ptr<Tex>> images;

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

        Dim size;

        string d;
        getline(level_file, d);
        size.w = atoi(d.c_str());
        getline(level_file, d);
        size.h = atoi(d.c_str());

        m = move( unique_ptr<PhysicsMap>( new PhysicsMap(size, 32) ) );

        set<EntityAABB *> collidingEntities;

        while(!level_file.eof())
        {
            string s;
            Pt p;

            getline(level_file, s, ',');
            if(level_file.eof())
                break;
            getline(level_file, d, ',');
            p.x = atoi(d.c_str());
            getline(level_file, d);
            p.y = atoi(d.c_str());

            SimpleImage *a = new SimpleImage( images.at(s), p );
            if( !m->place(a, collidingEntities) ) {
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

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new OldGameMap() );
}
