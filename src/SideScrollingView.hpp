/*
 * SideScrollingView.hpp
 *
 *  Created on: 25-Apr-2013
 *      Author: arjun
 */

#ifndef SIDESCROLLINGVIEW_HPP_
#define SIDESCROLLINGVIEW_HPP_

#include "PhysicsMap.hpp"

class SideScrollingView
{
public:
    PhysicsMap* physicsMap;
    Entity* pivot;

    SideScrollingView() : physicsMap(nullptr), pivot(nullptr) {}

    void render();
};

#endif /* SIDESCROLLINGVIEW_HPP_ */
