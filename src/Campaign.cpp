/*
 * Campaign.cpp
 */

#include "Elements.hpp"
#include "EntityMap.hpp"

class Campaign : public Entity
{
    EntityMap em;

public:
    Campaign() : em(Dim(1024, 600), 32) {
        PlaceholderEntityAABB *a = new PlaceholderEntityAABB(Rect( Pt(10, 10), Dim(100, 100) ));
        PlaceholderEntityAABB *b = new PlaceholderEntityAABB(Rect( Pt(200, 300), Dim(100, 100) ));

        em.place(a);
        em.place(b);
    }

    void step() {
        em.step();
    }
};

unique_ptr<Entity> newCampaign()
{
    return unique_ptr<Entity>( new Campaign() );
}
