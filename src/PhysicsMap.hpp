/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

#include "EntityMap.hpp"

class DynamicEntity : public Entity
{
public:
    Pt velocity;
    const float groundFriction, gravityFactor;

    DynamicEntity(DrawableAABB *d, Pt pos,
            const float groundfriction, const float gravityFactor) : Entity(d, pos),
        velocity(0.0f, 0.0f), groundFriction(groundfriction), gravityFactor(gravityFactor) {}

    virtual ~DynamicEntity() {}
};

class PhysicsMap
{
public:
    EntityMap entityMap;

    PhysicsMap(Dim worldSize, float optimizationFactor) :
        entityMap(worldSize, optimizationFactor) {}

    bool place(Entity *e, set<Entity *> &collidingEntities); // override EntityMap
    void remove(Entity *e); // override EntityMap

    void performPhysics();

private:
    set<DynamicEntity *> dynamicEntities;

    static float calculatePostFrictionHorizontalVelocity(float horizontalVelocity, float groundFriction);
};

#endif /* PHYSICSMAP_HPP_ */
