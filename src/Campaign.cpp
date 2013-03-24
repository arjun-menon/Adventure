/*
 * Campaign.cpp
 */

#include "Elements.hpp"
#include "PhysicsMap.hpp"

class Campaign : public Entity
{
    EntityMap em;
    PlaceholderEntityAABB *a, *b, *c;

public:
    Campaign() : em(Dim(1024, 600), 32) {
        a = new PlaceholderEntityAABB( Pt(10, 10)   , Dim(100, 100) );
        b = new PlaceholderEntityAABB( Pt(210, 210) , Dim(100, 100) );
        c = new PlaceholderEntityAABB( Pt(10, 110) , Dim(10, 10) );

        set<EntityAABB *> collidingEntities;
        em.place(a, collidingEntities);
        em.place(b, collidingEntities);
        em.place(c, collidingEntities);
    }

    void step() {
        em.step();

        set<EntityAABB *> collidingEntities;
        em.moveBy(b, Pt(0, 0), collidingEntities);
    }
};

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new Campaign() );
}
