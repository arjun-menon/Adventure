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
            [](unsigned char range) { return Sys()->random() % range + 1; };

    unsigned char range = 100;
    randomColor.r -= random_number(range);
    randomColor.g -= random_number(range);
    randomColor.b -= random_number(range);

    return randomColor;
}

void ColoredBox::drawAt(xy pos) {
    Sys()->drawBox(pos, size, fillColor, outlineColor, outlineThickness);
}

void Image::drawAt(xy pos) {
    Sys()->drawImage(*tex, pos, flip, angle);
}
