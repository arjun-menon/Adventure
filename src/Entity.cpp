/*
 * Entity.cpp
 */

#include "BaseFramework.hpp"

PlaceholderEntityAABB::PlaceholderEntityAABB(Rect rect) : EntityAABB(rect) {
    /*
     * Pick a random color
     *
     * we'd like a light shade, so we subtract a small number from Color's default values
     */
    std::function<unsigned char (unsigned char)> random_number = [](unsigned char range) { return Sys()->random() % range + 1; };

    unsigned char range = 100;
    randomColor.r -= random_number(range);
    randomColor.g -= random_number(range);
    randomColor.b -= random_number(range);
}

void PlaceholderEntityAABB::step() {
    Sys()->drawBox( rect.pos, rect.sz, randomColor, randomColor );
}
