/*
 * PhysicsMap.cpp
 */

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

void PhysicsMap::performPhysics()
{
    for(auto e : dynamicEntities)
    {
        limitVelocity(*e);

        static set<Entity *> collidingEntities; // let's reuse it (therefore static)

        /*
         * apply gravity
         */
        collidingEntities.clear();
        e->velocity += xy(0, -1 * e->dynamicChars.gravityFactor);
        if( !entityMap.moveBy(e, e->velocity, collidingEntities) ) {
            e->velocity.y = 0;
            groundContact = true;
        }
        else
            groundContact = false;

        /*
         * apply friction
         */
        collidingEntities.clear();
        e->velocity.x = calculatePostFrictionHorizontalVelocity(e->velocity.x, e->dynamicChars.groundFriction);

        entityMap.moveBy(e, e->velocity, collidingEntities);
    }
}

void PhysicsMap::limitVelocity(DynamicEntity &e)
{
    float sign = (e.velocity.x >= 0) ? 1 : -1;

    if( abs(e.velocity.x) > e.dynamicChars.maxHorizontalSpeed )
        e.velocity.x = sign * e.dynamicChars.maxHorizontalSpeed;
}

float PhysicsMap::calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction)
{
    if( abs(horizontalVelocity) < groundFriction )
        return 0;

    float direction = horizontalVelocity < 0 ? -1.0f : 1.0f;
    return horizontalVelocity - direction * groundFriction;
}

