/*
 * EntityMap.hpp
 */

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

#include "Matrix.hpp"

//#define SMOOTH_MOTION // Define this for tunneling-free motion in EntityMap.

/*
 * Entity - a DrawableAABB which has a position on an EntityMap.
 */
class Entity
{
public:
    DrawableAABB *drawable;
    xy pos;

    Entity(DrawableAABB *d, xy pos) : drawable(d), pos(pos) {}
    inline const Rect getRect() { return Rect(pos, drawable->getSize()); }

    virtual ~Entity() {}
};

/*
 * OptimizationMatrix - used to efficiently track neighboring entities.
 */
class OptimizationMatrix
{
public:
    inline OptimizationMatrix(xy mapSize, const unsigned int optimizationFactor) :
            mapSize(mapSize), optimizationFactor(optimizationFactor) {
        resizeMap(mapSize);
    }

    inline void resizeMap(xy newMapSize) {
        mapSize = newMapSize;
        xy matrixSize = mapSize/optimizationFactor + xy(1, 1);
        matrix.resize(matrixSize);
    }
    inline const xy& getMapSize() { return mapSize; }

    void insert(Entity *e);
    void erase(Entity *e);
    set<Entity *> getEntities(Rect region);

private:
    Matrix< set<Entity *> > matrix;

    xy mapSize;
    const unsigned int optimizationFactor;

    inline SubMatrixRef< set<Entity *> > submat(xy pos, xy sz) {
        xy start = pos / optimizationFactor;
        // NOTE: start + (sz / opt..) - xy(1, 1) will give us a different `end`.
        //       (pos + sz) below must be added first before being divided, for the right `end`.
        xy end = (pos + sz) / optimizationFactor - start + xy(1, 1);
        return SubMatrixRef< set<Entity *> >(matrix, Rect(start, end));
    }
};

/*
 * EntityMap - manage entities on a 2D plane, compute collisions, etc.
 */
class EntityMap
{
private:
    set<Entity *> entities;
    OptimizationMatrix optmat;

    inline bool isInsideMap(const Entity &e) {
        return Rect(xy(0,0), optmat.getMapSize()).isInside( Rect(e.pos, e.drawable->getSize()) );
    }

    bool computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities);

public:
    inline EntityMap(xy worldSize, unsigned int optimizationFactor) : optmat(worldSize, optimizationFactor) {}
    inline const xy& getMapSize() { return optmat.getMapSize(); }
    inline const set<Entity *> & getEntities() { return entities; }

    bool place(Entity *e, set<Entity *> &collidingEntities);
    void remove(Entity *e);

    bool move(Entity *e, xy newPos,  set<Entity *> &collidingEntities);
    bool moveTest(Entity *e, xy newPos);
    bool moveByApprox(Entity *e, xy distance, set<Entity *> &collidingEntities);
    bool moveBySmooth(Entity *e, xy distance, set<Entity *> &collidingEntities);
    inline bool moveBy(Entity *e, xy distance, set<Entity *> &collidingEntities) {
#ifdef SMOOTH_MOTION
        return moveBySmooth(e, distance, collidingEntities);
#else
        return moveByApprox(e, distance, collidingEntities);
#endif
    }
};

#endif /* ENTITYMAP_HPP_ */
