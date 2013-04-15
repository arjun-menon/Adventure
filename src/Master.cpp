/*
 * Master.cpp
 *
 * The "Master" entity, ie. the root entity.
 */

#include "BaseFramework.hpp"

class Master : public Entity
{
    bool loaded;

    unique_ptr<Entity> campaign;

public:
    Master() : loaded(false)
    {
        unique_ptr<Entity> newCampaign(); // defined in Campaign.cpp
        campaign = newCampaign();
    }

    void step() {
        campaign->step();
    }

    ~Master() {
    }
};

WindowProperties defaultWindowProperties()
{
    WindowProperties windowProperties;

    // Default window height & width:
    windowProperties.dim = Dim(1024, 600);
    windowProperties.fullscreen = false;

    // Title
    windowProperties.title = "Adventure";

    return windowProperties;
}

Entity* getMaster() {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master();
    return master;
}
