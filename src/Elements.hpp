/*
 * Elements.hpp
 */

#ifndef ELEMENTS_HPP_
#define ELEMENTS_HPP_

#include "GeometricPrimitives.hpp"

struct Color
{
    unsigned char r, g, b, a;
    Color(unsigned char r=255, unsigned char g=255, unsigned char b=255, unsigned char a=255) : r(r), g(g), b(b), a(a) {}
};

class Tex
{
public:
    virtual Dim getDim() const = 0;
    inline float w() const { return getDim().w; }
    inline float h() const { return getDim().h; }
    virtual ~Tex() {}
};

/*
 * Entity - something that can be drawn
 */
class Drawable
{
public:
    virtual void drawAt(Pt pos) = 0;
    virtual ~Drawable() {}
};




// add consts to Tex -- i.e. ptr<const Tex>




/*
 * DrawableAABB - a Drawable with a queryable rectangular size.
 *                an Axis-Aligned Bounding Box (AABB) Drawable.
 */
class DrawableAABB : public Drawable
{
public:
    virtual Dim getSize() = 0;
};


/*
 * SimpleImage - an image.
 */
class SimpleImage : public DrawableAABB
{
    const shared_ptr<const Tex> tex;
    const Dim tex_size;

public:
    inline SimpleImage(shared_ptr<const Tex> tex) : tex(tex), tex_size(tex->getDim()) {}
    inline Dim getSize() { return tex_size; }

    void drawAt(Pt pos);
};

/*
 * Draws a box - as a placeholder for an EntityAABB.
 */
class ColoredBox : public DrawableAABB
{
    static Color randomColor();

public:
    const Dim size;
    const Color color;

    inline ColoredBox(Dim size, Color color) : size(size), color(color) {}
    inline ColoredBox(Dim size) : ColoredBox(size, randomColor()) {}

    inline Dim getSize() { return size; }
    void drawAt(Pt pos);
};


/*
class Image : public EntityAABB
{
    Pt  pos;
    Dim size;

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

    inline const Pt&  getPos () { return pos;  }
    inline const Dim& getSize() { return size; }

    inline void setPos (Pt  pos)  { this->pos = pos; }
    inline void setFlip(bool flip) { this->flip = flip; }
    inline void setAngle(float angle) { this->angle = angle; }
};

class Animation : public EntityAABB
{
    Pt  pos;
    Dim size;

    // TODO Implement

public:
    inline const Pt&  getPos () { return pos;  }
    inline const Dim& getSize() { return size; }

    inline void setPos (Pt  pos)  { this->pos = pos; }
};
*/
// Sprite...

// TrailingCursor...

// Button...

#endif /* ELEMENTS_HPP_ */
