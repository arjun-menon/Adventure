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

    bool outside_bounds(const EntityAABB &e);

public:
    EntityMap(Dim newMapSize, unsigned int optimizationFactor) :
        optimizationFactor( static_cast<float>(optimizationFactor) )
    { setMapSize(newMapSize); }

    void setMapSize(Dim newMapSize);

    bool isInsideMap(const EntityAABB &e);

    set<EntityAABB *> computeIntersectingEntities(const EntityAABB *e);

    void place(EntityAABB *e);
    void remove(EntityAABB *e);

    void move(EntityAABB *e, Pt newPos);

    void step();
};

#endif /* ENTITYMAP_HPP_ */
