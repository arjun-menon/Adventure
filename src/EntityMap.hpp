/*
 * EntityMap.hpp
 */

#include "BaseFramework.hpp"

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

/*
 * Entity - a DrawableAABB which has a position on an EntityMap.
 */
class Entity
{
public:
    DrawableAABB *d;
    xy pos;

    Entity(DrawableAABB *d, xy pos) : d(d), pos(pos) {}

    virtual ~Entity() {}
};

template<class T>
struct matrix
{
    xy size;
    vector< vector<T> > m;

    matrix() : size(xy(0,0)) {}

    void resize(const xy &size)
    {
        this->size = size;

        if(size.x != m.size())
            m.resize( static_cast<unsigned int>(size.x) );

        for(auto col = m.begin(); col != m.end(); col++)
            if(size.y != col->size())
                col->resize( static_cast<unsigned int>(size.y) );

        this->size = size;
    }

    T& at(int x, int y) { return (m.at(x)).at(y); }
};

class OptimizationMatrix
{
public:
    inline OptimizationMatrix(xy mapSize, const float optimizationFactor) :
    mapSize(mapSize), optimizationFactor(optimizationFactor) { mat.resize( ceil( mapSize/optimizationFactor ) ); }

    inline void resizeMap(xy newMapSize) { mat.resize( ceil( (mapSize = newMapSize)/optimizationFactor ) ); }
    inline const xy& getMapSize() { return mapSize; }

    void insert(Entity *e);
    void erase(Entity *e);
    set<Entity *> getEntities(Rect region);

private:
    matrix< set<Entity *> > mat;

    xy mapSize;
    const float optimizationFactor;

    inline xy_int matrixPos(const xy &pos) { return floor_int( pos / optimizationFactor ); }
};

class EntityMap
{
public:
    inline EntityMap(xy worldSize, float optimizationFactor) : optmat(worldSize, optimizationFactor) {}
    inline const xy& getMapSize() { return optmat.getMapSize(); }
    inline const set<Entity *> & getEntities() { return entities; }

    bool place(Entity *e, set<Entity *> &collidingEntities);
    void remove(Entity *e);

    bool move(Entity *e, xy newPos,  set<Entity *> &collidingEntities);
    bool moveBy(Entity *e, xy distance,  set<Entity *> &collidingEntities);

private:
    set<Entity *> entities;
    OptimizationMatrix optmat;

    inline bool isInsideMap(const Entity &e) {
        return Rect(xy(0,0), optmat.getMapSize()).isInside( Rect(e.pos, e.d->getSize()) );
    }

    bool computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities);
};

#endif /* ENTITYMAP_HPP_ */
