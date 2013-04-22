/*
 * Entity.hpp
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

struct Color
{
    unsigned char r, g, b, a;
    Color(unsigned char r=255, unsigned char g=255, unsigned char b=255, unsigned char a=255) : r(r), g(g), b(b), a(a) {}
};

class Tex
{
public:
    virtual Dim getDim() = 0;
    inline float w() { return getDim().w; }
    inline float h() { return getDim().h; }
    virtual ~Tex() {}
};

/*
 * Entity - a class that implements `void step()`.
 */
struct Entity
{
    virtual void step() {}
    virtual ~Entity() {}
};

/*
 * Axis-Aligned Bounding Box (AABB) entity
 */
struct EntityAABB : public Entity
{
    Rect rect;

    inline EntityAABB(Rect rect) : rect(rect) {}
    inline EntityAABB(Pt pos, Dim sz) : rect( Rect(pos, sz) ) {}
};

/*
 * Draws a box - as a placeholder for an EntityAABB.
 */
class PlaceholderEntityAABB : public EntityAABB
{
    Color randomColor;

public:
    PlaceholderEntityAABB(Rect rect);
    PlaceholderEntityAABB(Pt pos, Dim sz) : PlaceholderEntityAABB( Rect(pos, sz) ) {}
    void step();
};

/*
 * Draws an image, nothing else.
 */
class SimpleImage : public EntityAABB
{
    shared_ptr<Tex> tex;

public:
    Pt pos;

    inline SimpleImage(shared_ptr<Tex> tex, Pt pos) :
        EntityAABB(pos, tex->getDim()),
        tex(tex), pos(pos) {}

    void step();
};

#endif /* ENTITY_HPP_ */
