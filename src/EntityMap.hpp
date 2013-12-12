/*
 * EntityMap.hpp
 */

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

#include "Matrix.hpp"

/*
 * Entity - a DrawableAABB which has a position on an EntityMap.
 */
class Entity
{
public:
    DrawableAABB *d;
    xy pos;

    Entity(DrawableAABB *d, xy pos) : d(d), pos(pos) {}
    inline const Rect getRect() { return Rect(pos, d->getSize()); }

    virtual ~Entity() {}
};

class EntityMap
{
public:
    inline EntityMap(xy worldSize, unsigned int optimizationFactor) : optmat(worldSize, optimizationFactor) {}
    inline const xy& getMapSize() { return optmat.getMapSize(); }
    inline const set<Entity *> & getEntities() { return entities; }

    bool place(Entity *e, set<Entity *> &collidingEntities);
    void remove(Entity *e);

    bool move(Entity *e, xy newPos,  set<Entity *> &collidingEntities);
    bool moveTest(Entity *e, xy newPos);
    bool moveBy(Entity *e, xy distance,  set<Entity *> &collidingEntities);

private:
    set<Entity *> entities;

    inline bool isInsideMap(const Entity &e) {
        return Rect(xy(0,0), optmat.getMapSize()).isInside( Rect(e.pos, e.d->getSize()) );
    }

    bool computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities);

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

        SubMatrixRef< set<Entity *> > submat(xy pos, xy sz) {
            xy start = pos / optimizationFactor;
            // NOTE: start + (sz / opt..) - xy(1, 1) will give us a different `end`.
            //       (pos + sz) below must be added first before being divided, for the right `end`.
            xy end = (pos + sz) / optimizationFactor - start + xy(1, 1);
            return SubMatrixRef< set<Entity *> >(matrix, Rect(start, end));
        }
    }
    optmat;
};

#endif /* ENTITYMAP_HPP_ */
