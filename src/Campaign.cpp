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

class Campaign : public Entity
{
    PhysicsMap m;
    PlaceholderEntityAABB *a, *b, *c;

public:
    Campaign() : m(Dim(1024, 600), 32) {
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

        dynamic_cast<DynamicEntityTrait&>(*c).velocity.x = -7.0f;
    }

    void step() {
        m.step();

        set<EntityAABB *> collidingEntities;
        //m.entityMap.moveBy(b, Pt(-5, 0), collidingEntities);
    }
};

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new Campaign() );
}
