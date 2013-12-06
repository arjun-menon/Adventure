/*
 * PhysicsMap.hpp
 */

#ifndef PHYSICSMAP_HPP_
#define PHYSICSMAP_HPP_

class DynamicEntityCharacteristics
{
public:
    const float groundFriction,
                gravityFactor,
                maxHorizontalSpeed,
                horizontalWalkStep,
                jumpStep,
                bouncyFactor;

    DynamicEntityCharacteristics(
            const float groundFriction,
            const float gravityFactor,
            const float maxHorizontalSpeed,
            const float horizontalWalkStep = 0.0f,
            const float jumpStep = 0.0f,
            const float bouncyFactor = 0.0f) :
                groundFriction(groundFriction),
                gravityFactor(gravityFactor),
                maxHorizontalSpeed(maxHorizontalSpeed),
                horizontalWalkStep(horizontalWalkStep),
                jumpStep(jumpStep),
                bouncyFactor( sanitizeBouncyFactor(bouncyFactor) ) {}

private:
    inline static float sanitizeBouncyFactor(float bouncyFactor) { return bouncyFactor > 1.0f ? 1.0f : bouncyFactor; }
};

class DynamicEntity : public Entity
{
    friend class PhysicsMap;

public:
    const DynamicEntityCharacteristics &dynamicChars;

    DynamicEntity(DrawableAABB *d, xy pos, const DynamicEntityCharacteristics &dynamicChars) :
        Entity(d, pos), dynamicChars(dynamicChars), velocity(0.0f, 0.0f) {}

    virtual ~DynamicEntity() {}

private:
    xy velocity;
};

class PhysicsMap
{
public:
    EntityMap entityMap;

    PhysicsMap(xy worldSize, float optimizationFactor) :
        entityMap(worldSize, optimizationFactor), groundContact(false) {}

    bool place(Entity *e, set<Entity *> &collidingEntities); // override EntityMap
    void remove(Entity *e); // override EntityMap

    void performPhysics();

    inline void jump(DynamicEntity *e) {
        if(groundContact)
            e->velocity.y += e->dynamicChars.jumpStep;
    }
    inline void walkLeft(DynamicEntity *e)  { e->velocity.x -= e->dynamicChars.horizontalWalkStep; }
    inline void walkRight(DynamicEntity *e) { e->velocity.x += e->dynamicChars.horizontalWalkStep; }

private:
    set<DynamicEntity *> dynamicEntities;
    bool groundContact;

    void limitVelocity(DynamicEntity &e);
};

#endif /* PHYSICSMAP_HPP_ */
