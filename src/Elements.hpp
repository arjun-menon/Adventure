/*
 * Elements.hpp
 */

#ifndef ELEMENTS_HPP_
#define ELEMENTS_HPP_

#include "GeometricPrimitives.hpp"

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
 * Entity - something that can be drawn
 */
class Drawable
{
public:
    virtual void drawAt(xy pos) = 0;
    virtual ~Drawable() {}
};

/*
 * DrawableAABB - a Drawable with a queryable rectangular size.
 *                an Axis-Aligned Bounding Box (AABB) Drawable.
 */
class DrawableAABB : public Drawable
{
public:
    virtual const xy getSize() const = 0;
};

/*
 * SimpleImage - an image.
 */
class SimpleImage : public DrawableAABB
{
    shared_ptr<Tex> tex;

public:
    inline SimpleImage(shared_ptr<Tex> tex) : tex(tex) {}
    inline const xy getSize() const { return tex->getSize(); }

    void drawAt(xy pos);
};

/*
 * Draws a box - as a placeholder for an EntityAABB.
 */
class ColoredBox : public DrawableAABB
{
    static Color randomColor();

public:
    const xy size;
    const Color color;

    inline ColoredBox(xy size, Color color) : size(size), color(color) {}
    inline ColoredBox(xy size) : size(size), color(randomColor()) {}

    inline const xy getSize() const { return size; }
    void drawAt(xy pos);
};


/*
class Image : public EntityAABB
{
    xy  pos;
    xy size;

    shared_ptr<Tex> tex;
    bool flip;
    float angle;

public:
    inline Image(shared_ptr<Tex> tex, bool flip=false, float angle=0.0f) :
        tex(tex), flip(flip), angle(angle) {}

    inline Image(string file, bool flip=false, float angle=0.0f) : flip(flip), angle(angle) {
        tex = Sys()->loadTex(file);
    }

    void step();

    inline const xy&  getPos () { return pos;  }
    inline const xy& getSize() { return size; }

    inline void setPos (xy  pos)  { this->pos = pos; }
    inline void setFlip(bool flip) { this->flip = flip; }
    inline void setAngle(float angle) { this->angle = angle; }
};

class Animation : public EntityAABB
{
    xy  pos;
    xy size;

    // TODO Implement

public:
    inline const xy&  getPos () { return pos;  }
    inline const xy& getSize() { return size; }

    inline void setPos (xy  pos)  { this->pos = pos; }
};
*/
// Sprite...

// TrailingCursor...

// Button...

class InputCallbacks
{
public:
    virtual void upKey() {}
    virtual void leftKey() {}
    virtual void rightKey() {}
    virtual void downKey() {}

    virtual ~InputCallbacks() {}
};

#endif /* ELEMENTS_HPP_ */
