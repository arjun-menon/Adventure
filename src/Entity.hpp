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
class Entity
{
public:
    virtual void step() {}
    virtual ~Entity() {}
};

/*
 * Entity that is a Axis-Aligned Bounding Box (AABB).
 */
class EntityAABB : public Entity
{
public:
    virtual const Pt&  getPos () = 0;
    virtual const Dim& getSize() = 0;
};

/*
 * Draws a box - as a placeholder for an EntityAABB.
 */
class PlaceholderEntityAABB : public EntityAABB
{
    Pt  pos;
    Dim size;

    Color randomColor;

public:
    PlaceholderEntityAABB(Pt pos, Dim size);
    void step();

    inline const Pt&  getPos () { return pos;  }
    inline const Dim& getSize() { return size; }

    inline void setPos (Pt  pos)  { this->pos = pos;   }
    inline void setSize(Dim size) { this->size = size; }
};

#endif /* ENTITY_HPP_ */
