/*
 * SideScrollingMap.hpp
 *
 *  Created on: Apr 23, 2013
 *      Author: arjun
 */

#ifndef SIDESCROLLINGMAP_HPP_
#define SIDESCROLLINGMAP_HPP_

#include "PhysicsMap.hpp"

class SideScrollingMap : public PhysicsMap
{

public:
    //using PhysicsMap::PhysicsMap; // Inheriting constructors (C++11 - N2540)
    SideScrollingMap(Dim worldSize, float optimizationFactor) :
        PhysicsMap(worldSize, optimizationFactor) {}

    void step()
    {
        performPhysics();

        // Invoke step() on each entity:
        for(auto e : entityMap.getEntities())
            e->d->drawAt(e->pos);
    }
};

#endif /* SIDESCROLLINGMAP_HPP_ */
