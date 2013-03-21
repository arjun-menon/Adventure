/*
 * Entity.cpp
 */

#include <functional>

#include "BaseFramework.hpp"

PlaceholderEntityAABB::PlaceholderEntityAABB(Rect rect) : EntityAABB(rect) {
    /*
     * Pick an outline color
     */
    std::function<unsigned char ()> pick_shade = []() { return Sys()->random() % 2 ? 255:0 ; };

    outlineColor.r = pick_shade();
    outlineColor.g = pick_shade();
    outlineColor.b = pick_shade();

    if( !outlineColor.r && !outlineColor.g && !outlineColor.b)
        outlineColor.r = outlineColor.g = outlineColor.b = 255;

    /*
     * Pick a fill color
     *
     * we'd like a light shade, so we subtract a small number from Color's default values
     */
    std::function<unsigned char (unsigned char)> random_number = [](unsigned char range) { return Sys()->random() % range + 1; };

    unsigned char magic = 100;
    fillColor.r -= random_number(magic);
    fillColor.g -= random_number(magic);
    fillColor.b -= random_number(magic);
}

void PlaceholderEntityAABB::step() {
    Sys()->drawBox( rect.pos, rect.sz, fillColor, outlineColor );
}
