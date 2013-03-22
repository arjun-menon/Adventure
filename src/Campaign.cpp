/*
 * Campaign.cpp
 */

#include "Elements.hpp"
#include "EntityMap.hpp"

class Campaign : public Entity
{
    EntityMap em;
    PlaceholderEntityAABB *a, *b;

public:
    Campaign() : em(Dim(1024, 600), 32) {
        a = new PlaceholderEntityAABB(Rect( Pt(10, 10), Dim(100, 100) ));
        b = new PlaceholderEntityAABB(Rect( Pt(200, 300), Dim(100, 100) ));

        em.place(a);
        em.place(b);
    }

    void step() {
        em.step();

        try {
            em.move(b, b->rect.pos + Pt(-1,-1));
        }
        catch(set<EntityAABB *> &intersectingEntities) {
        }
    }
};

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new Campaign() );
}
