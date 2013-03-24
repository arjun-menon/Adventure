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
	inline NAME& operator=(const NAME &other) { \
		A = other.A; \
		B = other.B; \
		return *this; \
	} \
	\
	inline NAME operator+(const NAME &other) const { \
		return NAME(A + other.A, B + other.B); \
	} \
	inline NAME& operator+=(const NAME& other) { \
		A += other.A; \
		B += other.B; \
		return *this; \
	} \
	\
	inline NAME operator-(const NAME &other) const { \
		return NAME(A - other.A, B - other.B); \
	} \
	inline NAME& operator-=(const NAME &other) { \
		A -= other.A; \
		B -= other.B; \
		return *this; \
	} \
	\
	inline NAME operator/(const T &divisor) const { \
        return NAME(A / divisor, B / divisor); \
    } \
    \
    inline NAME operator*(const T &multiplier) const { \
        return NAME(A * multiplier, B * multiplier); \
    } \
    \
    inline bool operator==(const NAME &other) const { \
        return A == other.A && B == other.B; \
    } \
    \
};

VEC2_TEMPLATE_GENERATOR(PtPolymorphic, x, y)
typedef PtPolymorphic<float> Pt;
typedef PtPolymorphic<int> PtI;

VEC2_TEMPLATE_GENERATOR(DimPolymorphic, w, h)
typedef DimPolymorphic<float> Dim;
typedef DimPolymorphic<int> DimI;

template <class T>
class RectPolymorphic
{
public:
    PtPolymorphic<T> pos;
    DimPolymorphic<T> sz;

	inline RectPolymorphic(PtPolymorphic<T> pos, DimPolymorphic<T> sz) :
	        pos(pos), sz(sz) {}

	inline bool isInside(const PtPolymorphic<T> &pt) const {
		return pt.x >= pos.x && pt.y >= pos.y
		       && pt.x <= (pos.x + sz.w - 1)
		       && pt.y <= (pos.y + sz.h - 1);
	}

	inline bool isInside(const RectPolymorphic<T> &rt) const {
	    // check if all of the four corners of rt are within this Rect
	    return isInside( rt.pos ) &&
	           isInside( rt.pos + Pt(rt.sz.w - 1, 0) ) &&
	           isInside( rt.pos + Pt(0, rt.sz.h - 1) ) &&
	           isInside( rt.pos + Pt(rt.sz.w - 1, rt.sz.h - 1) );
	}

	inline bool isPartiallyInside(const RectPolymorphic<T> &rt) const {
	    // check if any of the four corners of rt are within this Rect
        return isInside( rt.pos ) ||
               isInside( rt.pos + Pt(rt.sz.w - 1, 0) ) ||
               isInside( rt.pos + Pt(0, rt.sz.h - 1) ) ||
               isInside( rt.pos + Pt(rt.sz.w - 1, rt.sz.h - 1) );
	}

	inline bool doesIntersect(const RectPolymorphic<T> &rt) const {
	    return ( this->isPartiallyInside(rt) || rt.isPartiallyInside(*this) );
	}

	inline PtPolymorphic<T> getOppositeCorner() {
	    return PtPolymorphic<T>(pos.x + sz.w, pos.y + sz.h);
    }
};

typedef RectPolymorphic<float> Rect;

#endif /* GEOMETRICPRIMITIVES_HPP_ */
