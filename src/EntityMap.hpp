/*
 * EntityMap.hpp
 */

#include "BaseFramework.hpp"

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

/*
 * Entity - a polymorphic holder for a DrawableAABB
 *          that has a position on an EntityMap.
 */
struct Entity
{
    const shared_ptr<DrawableAABB> d;
    Pt pos;

    Entity(shared_ptr<DrawableAABB> d, Pt pos) : d(d), pos(pos) {}

    virtual ~Entity() {}
};

template<class T>
struct matrix
{
    Dim size;
    vector< vector<T> > m;

    matrix() : size(Dim(0,0)) {}

    void resize(const Dim &size)
    {
        this->size = size;

        if(size.w != m.size())
            m.resize( static_cast<unsigned int>(size.w) );

        for(auto col = m.begin(); col != m.end(); col++)
            if(size.h != col->size())
                col->resize( static_cast<unsigned int>(size.h) );

        this->size = size;
    }

    T& at(int x, int y) { return (m.at(x)).at(y); }
};

class OptimizationMatrix
{
    matrix< set<Entity *> > mat;

    Dim mapSize;
    const float optimizationFactor;

    inline PtI matrixBottomLeft(const Rect &rt) { return floor( rt.pos/optimizationFactor ); }
    inline PtI matrixTopRight(const Rect &rt)   { return floor( Pt(rt.pos.x + rt.sz.w, rt.pos.y + rt.sz.h)/optimizationFactor ); }

public:
    inline OptimizationMatrix(Dim mapSize, const float optimizationFactor) :
    mapSize(mapSize), optimizationFactor(optimizationFactor) { mat.resize( ceil( mapSize/optimizationFactor ) ); }

    inline void resizeMap(Dim newMapSize) { mat.resize( ceil( (mapSize = newMapSize)/optimizationFactor ) ); }
    inline Dim getMapSize() { return mapSize; }

    void insert(Entity *e);
    void erase(Entity *e);
    set<Entity *> getEntities(Rect region);
};

class EntityMap
{
    set<Entity *> entities;
    OptimizationMatrix optmat;

    inline bool isInsideMap(const Entity &e) {
        return Rect(Pt(0,0), optmat.getMapSize()).isInside( Rect(e.pos, e.d->getSize()) );
    }
    bool computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities);

protected:
    inline EntityMap(Dim worldSize, float optimizationFactor) : optmat(worldSize, optimizationFactor) {}

    virtual bool place(Entity *e, set<Entity *> &collidingEntities);
    virtual void remove(Entity *e);

    virtual ~EntityMap() {}

public:
    inline const set<Entity *> & getEntities() { return entities; }

    bool move(Entity *e, Pt newPos,  set<Entity *> &collidingEntities);
    bool moveBy(Entity *e, Pt distance,  set<Entity *> &collidingEntities);
};

#endif /* ENTITYMAP_HPP_ */
