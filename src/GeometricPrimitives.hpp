/*
 * GeometricPrimitives.hpp
 */

#ifndef GEOMETRICPRIMITIVES_HPP_
#define GEOMETRICPRIMITIVES_HPP_

#define VEC2_TEMPLATE_GENERATOR(NAME, A, B) \
template <class T> \
class NAME \
{ \
public: \
	T A, B; \
	\
	inline NAME() : A(0), B(0) {} \
	inline NAME(T A, T B) : A(A), B(B) {} \
	inline NAME(const NAME& other) : A(other.A), B(other.B) {} \
	\
	inline NAME& operator=(const NAME& other) { \
		A = other.A; \
		B = other.B; \
		return *this; \
	} \
	\
	inline NAME operator+(const NAME& other) const { \
		return NAME(A + other.A, B + other.B); \
	} \
	inline NAME& operator+=(const NAME& other) { \
		A += other.A; \
		B += other.B; \
		return *this; \
	} \
	\
	inline NAME operator-(const NAME& other) const { \
		return NAME(A - other.A, B - other.B); \
	} \
	inline NAME& operator-=(const NAME& other) { \
		A -= other.A; \
		B -= other.B; \
		return *this; \
	} \
	\
};

VEC2_TEMPLATE_GENERATOR(PtPolymorphic, x, y)
typedef PtPolymorphic<float> Pt;

VEC2_TEMPLATE_GENERATOR(DimPolymorphic, w, h)
typedef DimPolymorphic<float> Dim;

class Rect
{
public:
	Pt pos;
	Dim sz;

	inline Rect(Pt pos, Dim sz) : pos(pos), sz(sz) {}

	inline bool isInside(const Pt &pt) const {
		return pt.x >= pos.x && pt.y >= pos.y
		       && pt.x <= (pos.x + sz.w - 1)
		       && pt.y <= (pos.y + sz.h - 1);
	}

	inline bool isInside(const Rect &rt) const {
	    // check if all of the four corners of rt are within this Rect
	    return isInside( rt.pos ) &&
	           isInside( rt.pos + Pt(rt.sz.w - 1, 0) ) &&
	           isInside( rt.pos + Pt(0, rt.sz.h - 1) ) &&
	           isInside( rt.pos + Pt(rt.sz.w - 1, rt.sz.h - 1) );
	}

	inline bool isPartiallyInside(const Rect &rt) const {
	    // check if any of the four corners of rt are within this Rect
        return isInside( rt.pos ) ||
               isInside( rt.pos + Pt(rt.sz.w - 1, 0) ) ||
               isInside( rt.pos + Pt(0, rt.sz.h - 1) ) ||
               isInside( rt.pos + Pt(rt.sz.w - 1, rt.sz.h - 1) );
	}

	inline bool doesIntersect(const Rect &rt) const {
	    return ( this->isPartiallyInside(rt) || rt.isPartiallyInside(*this) );
	}
};

#endif /* GEOMETRICPRIMITIVES_HPP_ */
