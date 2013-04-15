/*
 * EntityMap.hpp
 */

#include "BaseFramework.hpp"

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

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
    matrix< set<EntityAABB *> > mat;

    Dim mapSize;
    const float optimizationFactor;

    inline PtI matrixBottomLeft(const Rect &rt) { return floor( rt.pos/optimizationFactor ); }
    inline PtI matrixTopRight(const Rect &rt)   { return floor( Pt(rt.pos.x + rt.sz.w, rt.pos.y + rt.sz.h)/optimizationFactor ); }

public:
    inline OptimizationMatrix(Dim mapSize, const float optimizationFactor) :
    mapSize(mapSize), optimizationFactor(optimizationFactor) { mat.resize( ceil( mapSize/optimizationFactor ) ); }

    inline void resizeMap(Dim newMapSize) { mat.resize( ceil( (mapSize = newMapSize)/optimizationFactor ) ); }
    inline Dim getMapSize() { return mapSize; }

    void insert(EntityAABB *e);
    void erase(EntityAABB *e);
    set<EntityAABB *> getEntities(Rect region);
};

class EntityMap : public Entity
{
    set<EntityAABB *> entities;
    OptimizationMatrix optmat;

    inline bool isInsideMap(const EntityAABB &e) { return Rect(Pt(0,0), optmat.getMapSize()).isInside(e.rect); }
    bool computeEntityCollisions(const EntityAABB *e, set<EntityAABB *> &collidingEntities);

public:
    EntityMap(Dim worldSize, float optimizationFactor) : optmat(worldSize, optimizationFactor) {}

    virtual bool place(EntityAABB *e, set<EntityAABB *> &collidingEntities);
    virtual void remove(EntityAABB *e);

    bool move(EntityAABB *e, Pt newPos,  set<EntityAABB *> &collidingEntities);
    bool moveBy(EntityAABB *e, Pt distance,  set<EntityAABB *> &collidingEntities);

    virtual void step();
};

#endif /* ENTITYMAP_HPP_ */
