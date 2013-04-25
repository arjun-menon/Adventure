/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

#include "EntityMap.hpp"

class DynamicEntityTrait
{
public:
    Pt velocity;
    const float groundFriction, gravityFactor;

    DynamicEntityTrait(const float groundfriction, const float gravityFactor) :
        velocity(0.0f, 0.0f), groundFriction(groundfriction), gravityFactor(gravityFactor) {}

    virtual ~DynamicEntityTrait() {}
};

class PhysicsMap : public EntityMap
{
public:
    PhysicsMap(Dim worldSize, float optimizationFactor) :
        EntityMap(worldSize, optimizationFactor) {}

    bool place(Entity *e, set<Entity *> &collidingEntities); // override EntityMap
    void remove(Entity *e); // override EntityMap

    void performPhysics();

private:
    set<Entity *> dynamicEntities;

    static float calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction);
};

#endif /* PHYSICSMAP_HPP_ */
