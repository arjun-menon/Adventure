/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

#include "EntityMap.hpp"

struct DynamicEntityTrait
{
    Pt velocity;
    const float groundFriction, gravityFactor;

    DynamicEntityTrait(const float groundfriction, const float gravityFactor) :
        velocity(0.0f, 0.0f), groundFriction(groundfriction), gravityFactor(gravityFactor) {}
};

class PhysicsMap
{
    set<EntityAABB *> dynamicEntities;

    static float calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction);

public:
    EntityMap entityMap;

    PhysicsMap(Dim worldSize, float optimizationFactor) :
        entityMap(worldSize, optimizationFactor) {}

    bool place(EntityAABB *e, set<EntityAABB *> &collidingEntities);
    void remove(EntityAABB *e);

    void step();
};

#endif /* PHYSICSMAP_HPP_ */
