/*
 * EntityMap.hpp
 */

#include <vector>

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

class EntityMap : public Entity
{
    Dim mapSize;

    matrix< set<EntityAABB *> > mat;
    const float optimizationFactor;

    set<EntityAABB *> entities;

    void setMapSize(Dim newMapSize) { mat.resize( ceil( (mapSize = newMapSize)/optimizationFactor ) ); }
    bool isInsideMap(const EntityAABB &e) { return Rect(Pt(0,0), mapSize).isInside(e.rect); }

    PtI matrixBottomLeft(const Rect &rt) { return floor( rt.pos/optimizationFactor ); }
    PtI matrixTopRight(const Rect &rt) { return floor( Pt(rt.pos.x + rt.sz.w, rt.pos.y + rt.sz.h)/optimizationFactor ); }

    bool computeEntityCollisions(const EntityAABB *e, set<EntityAABB *> &collidingEntities);

public:
    EntityMap(Dim worldSize, float optimizationFactor) :
        optimizationFactor(optimizationFactor) { setMapSize(worldSize); }

    virtual bool place(EntityAABB *e, set<EntityAABB *> &collidingEntities);
    virtual void remove(EntityAABB *e);

    bool move(EntityAABB *e, Pt newPos,  set<EntityAABB *> &collidingEntities);
    bool moveBy(EntityAABB *e, Pt distance,  set<EntityAABB *> &collidingEntities);

    virtual void step();
};

#endif /* ENTITYMAP_HPP_ */
