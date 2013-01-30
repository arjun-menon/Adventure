/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

class Master : public Entity
{
public:
    Master(WindowProperties &windowProperties) {
        windowProperties.title = "Adventure";
    }
    void step() {
        //
    }
    ~Master() {
    }
};

Entity* getMaster(WindowProperties &windowProperties) {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master(windowProperties);
    return master;
}
