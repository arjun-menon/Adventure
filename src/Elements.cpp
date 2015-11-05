/*
 * Elements.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"

/*
 * Pick a random color.
 *
 * We'd like a light shade, so we subtract a small random number from Color's default values
 */
Color ColoredBox::randomColor() {
    Color randomColor;

    std::function<unsigned char (unsigned char)> random_number =
            [](unsigned char range) { return sys->random() % range + 1; };

    unsigned char k = 50;
    randomColor.r -= k + random_number(k);
    randomColor.g -= k + random_number(k);
    randomColor.b -= k + random_number(k);

    return randomColor;
}

void ColoredBox::drawAt(xy pos) {
    sys->drawBox(pos, size, fillColor, outlineColor, outlineThickness);
}

void Image::drawAt(xy pos) {
    sys->drawImage(*tex, pos, flip, angle);
}
