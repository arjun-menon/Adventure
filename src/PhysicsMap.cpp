/*
 * PhysicsMap.cpp
 */

#include "PhysicsMap.hpp"

bool PhysicsMap::place(EntityAABB *e, set<EntityAABB *> &collidingEntities)
{
    if( !entityMap.place(e, collidingEntities) )
        return false;

    if( dynamic_cast<DynamicEntityTrait *>(e) != nullptr )
        dynamicEntities.insert(e);

    return true;
}

void PhysicsMap::remove(EntityAABB *e)
{
    if( dynamic_cast<DynamicEntityTrait *>(e) != nullptr )
        dynamicEntities.erase(e);

    entityMap.remove(e);
}

float PhysicsMap::applyGroundFriction(float horizontalVelocity, float groundFriction)
{
    if( abs(horizontalVelocity) < groundFriction )
        return 0;

    float direction = horizontalVelocity < 0 ? -1.0f : 1.0f;
    return horizontalVelocity - direction * groundFriction;
}

void PhysicsMap::step()
{
    for(auto e : dynamicEntities)
    {
        DynamicEntityTrait *dynamicTrait = dynamic_cast<DynamicEntityTrait *>(e);

        if(dynamicTrait == nullptr)
            throw logic_error("Non-dynamic entity in PhysicsMap.dynamicEntities set");

        static set<EntityAABB *> collidingEntities; // let's reuse it (therefore static)

        /*
         * apply gravity
         */
        collidingEntities.clear();
        dynamicTrait->velocity += Pt(0, -1 * dynamicTrait->gravityFactor);
        if( !entityMap.moveBy(e, dynamicTrait->velocity, collidingEntities) )
            dynamicTrait->velocity.y = 0;

        /*
         * apply friction
         */
        collidingEntities.clear();
        dynamicTrait->velocity.x = applyGroundFriction(dynamicTrait->velocity.x, dynamicTrait->groundFriction);
        entityMap.moveBy(e, dynamicTrait->velocity, collidingEntities);
    }

    entityMap.step();
}
