/*
 * Entity.cpp
 */

#include <functional>

#include "BaseFramework.hpp"

PlaceholderEntityAABB::PlaceholderEntityAABB(Rect rect) : EntityAABB(rect) {
    std::function<unsigned char ()> pick_shade = []() { return Sys()->random() % 2 ? 255:0 ; };
    randomColor.r = pick_shade();
    randomColor.g = pick_shade();
    randomColor.b = pick_shade();

    if( !randomColor.r && !randomColor.g && !randomColor.b)
        randomColor.r = randomColor.g = randomColor.b = 255;
}

void PlaceholderEntityAABB::step() {
    Sys()->drawBox( rect.pos, rect.sz, Color(255,255,255,0), randomColor );
}
