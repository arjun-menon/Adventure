/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

class Master : public Entity
{
    bool loaded;

    unique_ptr<Entity> campaign;

public:
    Master() : loaded(false)
    {
        Sys()->windowProperties.title = "Adventure";

        unique_ptr<Entity> newCampaign(); // prototype
        campaign = newCampaign();
    }

    void step() {
        campaign->step();
    }

    ~Master() {
    }
};

Entity* getMaster() {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master();
    return master;
}
