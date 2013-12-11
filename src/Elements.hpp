/*
 * Elements.hpp
 */

#ifndef ELEMENTS_HPP_
#define ELEMENTS_HPP_

/*
 * Color - RBG color with alpha channel.
 */
struct Color
{
    unsigned char r, g, b, a;
    Color(unsigned char r=255, unsigned char g=255, unsigned char b=255, unsigned char a=255) : r(r), g(g), b(b), a(a) {}
};

/*
 * Tex - a 2D texture.
 */
class Tex
{
public:
    virtual const xy getSize() const = 0;
    virtual ~Tex() {}
};

/*
 * Drawable - something that can be drawn
 */
class Drawable
{
public:
    virtual void drawAt(xy pos) = 0;
    virtual ~Drawable() {}
};

/*
 * DrawableAABB - a Drawable xis-Aligned Bounding Box (AABB)
 *                        with a queryable height & width.
 */
class DrawableAABB : public Drawable
{
public:
    virtual const xy getSize() const = 0;
};

/*
 * A colored box - could be used as a placeholder for an image.
 */
class ColoredBox : public DrawableAABB
{
    const xy size;

    static Color randomColor();

public:
    Color fillColor, outlineColor;
    float outlineThickness;

    inline ColoredBox(xy size, Color fillColor=randomColor(), Color outlineColor=randomColor(), float outlineThickness=1.0f) : 
            size(size), fillColor(fillColor), outlineColor(outlineColor), outlineThickness(outlineThickness) {}

    const xy getSize() const { return size; }
    void drawAt(xy pos);
};

/*
 * Image - an image.
 */
class Image : public DrawableAABB
{
    shared_ptr<Tex> tex;

public:
    bool flip;
    float angle;

    inline Image(shared_ptr<Tex> tex, bool flip=false, float angle=0.0f) : 
        tex(tex), flip(flip), angle(angle) {}

    const xy getSize() const { return tex->getSize(); }
    void drawAt(xy pos);
};

// Animated Sprite...

// TrailingCursor...

// Button...

#endif /* ELEMENTS_HPP_ */
