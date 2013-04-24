/*
 * PhysicsMap.cpp
 */

#include "PhysicsMap.hpp"

bool PhysicsMap::place(Entity *e, set<Entity *> &collidingEntities)
{
    if( !EntityMap::place(e, collidingEntities) )
        return false;

    if( dynamic_cast<DynamicEntityTrait *>(e) != nullptr )
        dynamicEntities.insert(e);

    return true;
}

void PhysicsMap::remove(Entity *e)
{
    if( dynamic_cast<DynamicEntityTrait *>(e) != nullptr )
        dynamicEntities.erase(e);

    EntityMap::remove(e);
}

float PhysicsMap::calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction)
{
    if( abs(horizontalVelocity) < groundFriction )
        return 0;

    float direction = horizontalVelocity < 0 ? -1.0f : 1.0f;
    return horizontalVelocity - direction * groundFriction;
}

void PhysicsMap::performPhysics()
{
    for(auto e : dynamicEntities)
    {
        DynamicEntityTrait *dynamicTrait = dynamic_cast<DynamicEntityTrait *>(e);

        if(dynamicTrait == nullptr)
            throw logic_error("Non-dynamic entity in PhysicsMap.dynamicEntities set");

        static set<Entity *> collidingEntities; // let's reuse it (therefore static)

        /*
         * apply gravity
         */
        collidingEntities.clear();
        dynamicTrait->velocity += Pt(0, -1 * dynamicTrait->gravityFactor);
        if( !moveBy(e, dynamicTrait->velocity, collidingEntities) )
            dynamicTrait->velocity.y = 0;

        /*
         * apply friction
         */
        collidingEntities.clear();
        dynamicTrait->velocity.x = calculatePostFrictionHorizontalVelocity(dynamicTrait->velocity.x, dynamicTrait->groundFriction);
        moveBy(e, dynamicTrait->velocity, collidingEntities);
    }
}
