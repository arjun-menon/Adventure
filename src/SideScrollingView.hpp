/*
 * SideScrollingView.hpp
 */

#ifndef SIDESCROLLINGVIEW_HPP_
#define SIDESCROLLINGVIEW_HPP_

class SideScrollingView
{
public:
    PhysicsMap* physicsMap;
    Entity* player;

    SideScrollingView() : physicsMap(nullptr), player(nullptr) {}

    void render();
};

#endif /* SIDESCROLLINGVIEW_HPP_ */
