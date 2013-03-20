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
};

/*
 * Draws a box - as a placeholder for an EntityAABB.
 */
class PlaceholderEntityAABB : public EntityAABB
{
    Color randomColor;

public:
    PlaceholderEntityAABB(Rect rect);
    void step();
};

#endif /* ENTITY_HPP_ */
