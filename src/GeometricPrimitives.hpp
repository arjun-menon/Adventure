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

    #define PT_SZ 1 // Size of a "point" (defined here as the size of one pixel).

    inline RectPolymorphic(PtPolymorphic<T> pos, DimPolymorphic<T> sz) :
            pos(pos), sz(sz) {}

    inline bool isInside(const PtPolymorphic<T> &pt) const {
        return pt.x >= pos.x && pt.y >= pos.y
            && pt.x <= (pos.x + sz.w - PT_SZ)
            && pt.y <= (pos.y + sz.h - PT_SZ);
    }

    inline bool isInside(const RectPolymorphic<T> &rt) const {
        // check if all of the four corners of rt are within this Rect
        return isInside( rt.pos ) &&
               isInside( rt.pos + Pt(rt.sz.w - PT_SZ, 0) ) &&
               isInside( rt.pos + Pt(0, rt.sz.h - PT_SZ) ) &&
               isInside( rt.pos + Pt(rt.sz.w - PT_SZ, rt.sz.h - PT_SZ) );
    }

    inline bool isPartiallyInside(const RectPolymorphic<T> &rt) const {
        // check if any of the four corners of rt are within this Rect
        return isInside( rt.pos ) ||
               isInside( rt.pos + Pt(rt.sz.w - PT_SZ, 0) ) ||
               isInside( rt.pos + Pt(0, rt.sz.h - PT_SZ) ) ||
               isInside( rt.pos + Pt(rt.sz.w - PT_SZ, rt.sz.h - PT_SZ) );
    }

    inline bool doesIntersect(const RectPolymorphic<T> &rt) const {
        return ( this->isPartiallyInside(rt) || rt.isPartiallyInside(*this) );
    }

    inline PtPolymorphic<T> getOppositeCorner() {
        return PtPolymorphic<T>(pos.x + sz.w, pos.y + sz.h);
    }

    #undef PT_SZ
};

typedef RectPolymorphic<float> Rect;

/*
 * Define functions that apply a specified function to the members of a VEC2.
 */

#define MATH_TRANSFORM_FUNCTION(FUNCTION, SOURCE_TYPE, TARGET_TYPE, A, B) \
inline TARGET_TYPE FUNCTION(SOURCE_TYPE source) { \
    return TARGET_TYPE( FUNCTION(source.A), FUNCTION(source.B) ); \
}

#define MATH_TRANSFORM_FUNCTION_WITH_CAST(FUNCTION, SOURCE_TYPE, TARGET_TYPE, TARGET_SUBTYPE, A, B) \
inline TARGET_TYPE FUNCTION(SOURCE_TYPE source) { \
    return TARGET_TYPE( static_cast<TARGET_SUBTYPE>( FUNCTION(source.A) ),   \
                        static_cast<TARGET_SUBTYPE>( FUNCTION(source.B) ) ); \
}

MATH_TRANSFORM_FUNCTION_WITH_CAST(ceil, Pt, PtI, int, x, y)
MATH_TRANSFORM_FUNCTION_WITH_CAST(floor, Pt, PtI, int, x, y)
MATH_TRANSFORM_FUNCTION(ceil, Dim, Dim, w, h)
MATH_TRANSFORM_FUNCTION(floor, Dim, Dim, w, h)

#endif /* GEOMETRICPRIMITIVES_HPP_ */
