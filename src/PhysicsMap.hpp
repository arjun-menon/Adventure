/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

struct DynamicEntityCharacteristics
{
    const int groundFriction,
                gravityFactor,
                maxHorizontalSpeed,
                horizontalWalkStep,
                jumpStep;

    DynamicEntityCharacteristics(
            const int groundFriction,
            const int gravityFactor,
            const int maxHorizontalSpeed,
            const int horizontalWalkSpeed = 0,
            const int jumpStep = 0) :
                groundFriction(groundFriction),
                gravityFactor(gravityFactor),
                maxHorizontalSpeed(maxHorizontalSpeed),
                horizontalWalkStep(groundFriction + horizontalWalkSpeed),
                jumpStep(jumpStep) {}
};

class DynamicEntity : public Entity
{
    friend class PhysicsMap;

public:
    const DynamicEntityCharacteristics dynamicChars;

    DynamicEntity(DrawableAABB *d, xy pos, const DynamicEntityCharacteristics &dynamicChars) :
        Entity(d, pos), dynamicChars(dynamicChars), velocity(0.0f, 0.0f) {}

    virtual ~DynamicEntity() {}

private:
    xy velocity;
};

class PhysicsMap
{
    const static int defaultOptimizationFactor = 32;

public:
    EntityMap entityMap;

    PhysicsMap(xy worldSize, int optimizationFactor = defaultOptimizationFactor) :
        entityMap(worldSize, optimizationFactor) {}

    bool place(Entity *e, set<Entity *> &collidingEntities); // override EntityMap
    void remove(Entity *e); // override EntityMap

    void performPhysics();

    inline void jump(DynamicEntity *e) {
        if(!entityMap.moveTest(e, e->pos - xy(0, 1)))
            e->velocity.y += e->dynamicChars.jumpStep;
    }
    inline void walkLeft(DynamicEntity *e) {
        e->velocity.x -= e->dynamicChars.horizontalWalkStep;
    }
    inline void walkRight(DynamicEntity *e) {
        e->velocity.x += e->dynamicChars.horizontalWalkStep;
    }

private:
    set<DynamicEntity *> dynamicEntities;

    void limitVelocity(DynamicEntity &e);
};

#endif /* PHYSICSMAP_HPP_ */
