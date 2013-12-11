/*
 * PhysicsMap.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"
#include "EntityMap.hpp"
#include "PhysicsMap.hpp"

bool PhysicsMap::place(Entity *e, set<Entity *> &collidingEntities)
{
    if( !entityMap.place(e, collidingEntities) )
        return false;

    if( dynamic_cast<DynamicEntity *>(e) != nullptr )
        dynamicEntities.insert( dynamic_cast<DynamicEntity *>(e) );

    return true;
}

void PhysicsMap::remove(Entity *e)
{
    if( dynamic_cast<DynamicEntity *>(e) != nullptr )
        dynamicEntities.erase( dynamic_cast<DynamicEntity *>(e) );

    entityMap.remove(e);
}

static int calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction)
{
    if( abs(horizontalVelocity) < groundFriction )
        return 0;

    int direction = horizontalVelocity < 0 ? -1 : 1;
    return horizontalVelocity - direction * groundFriction;
}

void PhysicsMap::performPhysics()
{
    for(auto e : dynamicEntities)
    {
        limitVelocity(*e);

        int &x = e->velocity.x,
            &y = e->velocity.y;

        x = calculatePostFrictionHorizontalVelocity(x, e->dynamicChars.groundFriction); // apply friction
        y -= e->dynamicChars.gravityFactor; // apply gravity

        static set<Entity *> collidingEntities; // let's reuse it (therefore static)
        collidingEntities.clear();

        if( !entityMap.moveBy(e, e->velocity, collidingEntities) ) {
            // There has been a collision...

            collidingEntities.clear();
            if( !entityMap.moveBy(e, xy(0, y), collidingEntities) )
                y = 0;

            collidingEntities.clear(); // FIXME
            if( !entityMap.moveBy(e, xy(x, 0), collidingEntities) )
                x = 0;
        }
    }
}

void PhysicsMap::limitVelocity(DynamicEntity &e)
{
    float sign = (e.velocity.x >= 0) ? 1 : -1;

    if( abs(e.velocity.x) > e.dynamicChars.maxHorizontalSpeed )
        e.velocity.x = sign * e.dynamicChars.maxHorizontalSpeed;
}
