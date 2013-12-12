/*
 * GeometricPrimitives.hpp
 */

#ifndef GEOMETRICPRIMITIVES_HPP_
#define GEOMETRICPRIMITIVES_HPP_

template <typename T>
class xyPolymorphic
{
public:
    T x, y;

    inline xyPolymorphic() : x(0), y(0) {}
    inline xyPolymorphic(T x, T y) : x(x), y(y) {}
    inline xyPolymorphic(const xyPolymorphic& other) : x(other.x), y(other.y) {}

    inline xyPolymorphic& operator=(const xyPolymorphic &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    inline xyPolymorphic operator+(const xyPolymorphic &other) const {
        return xyPolymorphic(x + other.x, y + other.y);
    }
    inline xyPolymorphic& operator+=(const xyPolymorphic& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline xyPolymorphic operator-(const xyPolymorphic &other) const {
        return xyPolymorphic(x - other.x, y - other.y);
    }
    inline xyPolymorphic& operator-=(const xyPolymorphic &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline xyPolymorphic operator/(const T &divisor) const {
        return xyPolymorphic(x / divisor, y / divisor);
    }

    inline xyPolymorphic operator*(const T &multiplier) const {
        return xyPolymorphic(x * multiplier, y * multiplier);
    }

    inline bool operator==(const xyPolymorphic &other) const {
        return x == other.x && y == other.y;
    }

    inline string toString() const {
        stringstream s;
        s<<"(x = "<<x<<", "<<"y = "<<y<<")";
        return s.str();
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &strm, const xyPolymorphic<T> &k) {
    return strm << "(" << k.x << ", " << k.y << ")";
}

template <typename T>
class RectPolymorphic
{
public:
    xyPolymorphic<T> pos, size;

    #define PT_SZ 1 // Size of a "point" (defined here as the size of one pixel).

    inline RectPolymorphic(xyPolymorphic<T> pos, xyPolymorphic<T> size) :
            pos(pos), size(size) {}

    inline bool isInside(const xyPolymorphic<T> &pt) const {
        return pt.x >= pos.x && pt.y >= pos.y
            && pt.x <= (pos.x + size.x + PT_SZ)
            && pt.y <= (pos.y + size.y + PT_SZ);
    }

    inline bool isInside(const RectPolymorphic<T> &rect) const {
        // check if all of the four corners of rect are within this Rect
        return isInside( rect.pos ) &&
               isInside( rect.pos + xyPolymorphic<T>(rect.size.x + PT_SZ, 0) ) &&
               isInside( rect.pos + xyPolymorphic<T>(0, rect.size.y + PT_SZ) ) &&
               isInside( rect.pos + xyPolymorphic<T>(rect.size.x + PT_SZ, rect.size.y + PT_SZ) );
    }

    inline bool isPartiallyInside(const RectPolymorphic<T> &rect) const {
        // check if any of the four corners of rect are within this Rect
        return isInside( rect.pos ) ||
               isInside( rect.pos + xyPolymorphic<T>(rect.size.x + PT_SZ, 0) ) ||
               isInside( rect.pos + xyPolymorphic<T>(0, rect.size.y + PT_SZ) ) ||
               isInside( rect.pos + xyPolymorphic<T>(rect.size.x + PT_SZ, rect.size.y + PT_SZ) );
    }

    inline bool doesIntersect(const RectPolymorphic<T> &rect) const {
        return ( this->isPartiallyInside(rect) || rect.isPartiallyInside(*this) );
    }

    inline xyPolymorphic<T> getOppositeCorner() const {
        return xyPolymorphic<T>(pos.x + size.x, pos.y + size.y);
    }

    #undef PT_SZ
};

template <typename T>
inline std::ostream& operator<<(std::ostream &strm, const RectPolymorphic<T> &rect) {
    return strm << "[pos" << rect.pos << ", size" << rect.size << "]";
}

/*
 * A few math functions for xyPolymorphic-derived types.
 */

inline xyPolymorphic<float> ceil(xyPolymorphic<float> val) {
    return xyPolymorphic<float>( ceil(val.x), ceil(val.y) );
}

inline xyPolymorphic<float> floor(xyPolymorphic<float> val) {
    return xyPolymorphic<float>( floor(val.x), floor(val.y) );
}

inline xyPolymorphic<int> ceil_int(xyPolymorphic<float> val) {
    return xyPolymorphic<int>( static_cast<int>( ceil(val.x) ), static_cast<int>( ceil(val.y) ) );
}

inline xyPolymorphic<int> floor_int(xyPolymorphic<float> val) {
    return xyPolymorphic<int>( static_cast<int>( floor(val.x) ), static_cast<int>( floor(val.y) ) );
}

typedef xyPolymorphic<int> xy;
typedef xyPolymorphic<float> xyf;

inline xyPolymorphic<float> xy_i2f(xyPolymorphic<int> k) {
    return xyf(static_cast<float>(k.x), static_cast<float>(k.y));
}
inline xyPolymorphic<int> xy_f2i(xyPolymorphic<float> k) {
    return xy(static_cast<int>(k.x), static_cast<int>(k.y));
}

typedef RectPolymorphic<int> Rect;
typedef RectPolymorphic<float> Rectf;

#endif /* GEOMETRICPRIMITIVES_HPP_ */
