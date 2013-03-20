/*
 * Elements.hpp
 */

#ifndef ELEMENTS_HPP_
#define ELEMENTS_HPP_

#include "BaseFramework.hpp"
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
*/
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

// Sprite...

// TrailingCursor...

// Button...

#endif /* ELEMENTS_HPP_ */
