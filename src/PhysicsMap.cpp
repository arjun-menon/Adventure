/*
 * PhysicsMap.cpp
 */

#include "PhysicsMap.hpp"

bool PhysicsMap::place(Entity *e, set<Entity *> &collidingEntities)
{
    cout<<"placing: "<< e <<endl;

    if( !EntityMap::place(e, collidingEntities) )
        return false;

    if( dynamic_cast<DynamicEntity *>(e) != nullptr )
        dynamicEntities.insert( dynamic_cast<DynamicEntity *>(e) );

    return true;
}

void PhysicsMap::remove(Entity *e)
{
    if( dynamic_cast<DynamicEntity *>(e) != nullptr )
        dynamicEntities.erase( dynamic_cast<DynamicEntity *>(e) );

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
        cout<<"dynamic: "<< e <<endl;
//        DynamicEntityTrait *dynamicTrait = dynamic_cast<DynamicEntityTrait *>(e);
//
//        if(dynamicTrait == nullptr)
//            throw logic_error("Non-dynamic entity in PhysicsMap.dynamicEntities set");

        static set<Entity *> collidingEntities; // let's reuse it (therefore static)

        /*
         * apply gravity
         */
        collidingEntities.clear();
        e->velocity += Pt(0, -1 * e->gravityFactor);
        if( !moveBy(e, e->velocity, collidingEntities) )
            e->velocity.y = 0;

        /*
         * apply friction
         */
        collidingEntities.clear();
        e->velocity.x = calculatePostFrictionHorizontalVelocity(e->velocity.x, e->groundFriction);
        moveBy(e, e->velocity, collidingEntities);
    }
}
