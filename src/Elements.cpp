/*
 * Elements.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"

void ColoredBox::drawAt(xy pos) {
    sys->drawBox(pos, size, fillColor, outlineColor, outlineThickness);
}

void Image::drawAt(xy pos) {
    sys->drawImage(*tex, pos, flip, angle);
}

/*
 * Pick a random light shade of color.
 */
Color ColoredBox::randomColor() {
    Color randomColor;

    const unsigned char k = 50; // subtracted for a light shade
    randomColor.r -= k + sys->random(k);
    randomColor.g -= k + sys->random(k);
    randomColor.b -= k + sys->random(k);

    return randomColor;
}
