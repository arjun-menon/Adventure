/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

#include "EntityMap.hpp"

class StaticPhysicsEntityTrait
{
    // we'll use RTTI to recognise an Entity is of this type.
};

class DynamicPhysicsEntityTrait
{
    int groundfriction, gravityFactor;

    DynamicPhysicsEntityTrait(int groundfriction, int gravityFactor) :
        groundfriction(groundfriction), gravityFactor(gravityFactor) {}
};

class PhysicsMap : public Entity
{
    EntityMap entityMap;

public:
    PhysicsMap(Dim worldSize, float optimizationFactor) :
        entityMap(worldSize, optimizationFactor) {}
};

#endif /* PHYSICSMAP_HPP_ */
