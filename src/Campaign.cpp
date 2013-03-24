/*
 * Campaign.cpp
 */

#include "Elements.hpp"
#include "PhysicsMap.hpp"

class MovingObject : public PlaceholderEntityAABB, public DynamicEntityTrait
{
public:
    MovingObject(Pt pos, Dim sz) : PlaceholderEntityAABB(pos, sz), DynamicEntityTrait(1.0f, 0.2f) {}
};

class Campaign : public Entity
{
    PhysicsMap m;
    PlaceholderEntityAABB *a, *b, *c;

public:
    Campaign() : m(Dim(1024, 600), 32) {
        a = new PlaceholderEntityAABB( Pt(10, 10)   , Dim(200, 100) );
        b = new MovingObject( Pt(220, 210) , Dim(100, 100) );
        c = new MovingObject( Pt(10, 110) , Dim(10, 10) );

        set<EntityAABB *> collidingEntities;
        m.place(a, collidingEntities);
        m.place(b, collidingEntities);
        m.place(c, collidingEntities);

        dynamic_cast<DynamicEntityTrait&>(*c).velocity.x = 15.0f;
    }

    void step() {
        m.step();
    }
};

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new Campaign() );
}
