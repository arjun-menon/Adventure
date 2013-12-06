/*
 * SideScrollingView.hpp
 */

#ifndef SIDESCROLLINGVIEW_HPP_
#define SIDESCROLLINGVIEW_HPP_

class SideScrollingView
{
public:
    PhysicsMap* physicsMap;
    Entity* pivot;

    SideScrollingView() : physicsMap(nullptr), pivot(nullptr) {}

    void render();
};

#endif /* SIDESCROLLINGVIEW_HPP_ */
