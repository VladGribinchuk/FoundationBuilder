#pragma once

#include <limits>
#include <cmath>
#include "Units.h"

namespace geom_utils
{
    template<typename T>
    struct Point2D
    {
        constexpr Point2D(T x = T{}, T y = T{})
            : x(x), y(y)
        {}
        T x, y;

        typedef T coord;
    };

    template<typename T>
    struct Point3D
    {
        constexpr Point3D(T x = T{}, T y = T{}, T z = T{})
            : x(x), y(y), z(z)
        {}
        T x, y, z;

        typedef T coord;
    };

    using FPoint2D = Point2D<mm>;
    using FPoint3D = Point3D<mm>;

    // Point2D related:

    template<typename Pnt>
    constexpr Pnt minPoint() { static_assert(false, "Not implemented yet for this type of Point"); return {}; }
    
    template<typename Pnt>
    constexpr Pnt maxPoint() { static_assert(false, "Not implemented yet for this type of Point"); return {}; }

    template<>
    constexpr FPoint2D minPoint() { return FPoint2D(std::numeric_limits<FPoint2D::coord>::min(), std::numeric_limits<FPoint2D::coord>::min()); }

    template<>
    constexpr FPoint2D maxPoint() { return FPoint2D(std::numeric_limits<FPoint2D::coord>::max(), std::numeric_limits<FPoint2D::coord>::max()); }


    template<typename T>
    constexpr Point2D<T> operator-(const Point2D<T>& p) { return Point2D<T>(-p.x, -p.y); }

    template<typename T>
    constexpr Point2D<T> operator+(const Point2D<T>& p, const Point2D<T>& p2) { return Point2D<T>(p.x + p2.x, p.y + p2.y); }

    template<typename T>
    constexpr Point2D<T> operator-(const Point2D<T>& p, const Point2D<T>& p2) { return Point2D<T>(p.x - p2.x, p.y - p2.y); }

    template<typename T>
    constexpr Point2D<T>& operator+=(Point2D<T>& p, const Point2D<T>& p2) { p.x += p2.x; p.y += p2.y; return p; }

    template<typename T>
    constexpr Point2D<T>& operator-=(Point2D<T>& p, const Point2D<T>& p2) { p.x -= p2.x; p.y -= p2.y; return p; }

    template<typename T, typename V>
    constexpr Point2D<T>  operator*(const Point2D<T>& p, const V val) { return Point2D<T>(p.x * val, p.y * val); }

    template<typename T, typename V>
    constexpr Point2D<T>  operator*(const V i, const Point2D<T>& p) { return p * i; }

    template<typename T>
    constexpr bool operator==(const Point2D<T>& lhs, const Point2D<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

    template<typename T>
    constexpr bool operator!=(const Point2D<T>& lhs, const Point2D<T>& rhs) { return !(lhs == rhs); }

    // special treatment for float-point number
    template<>
    inline bool operator==(const Point2D<float>& lhs, const Point2D<float>& rhs) { return std::fabs(lhs.x - rhs.x) < getUnitTolerance() && std::fabs(lhs.y - rhs.y) < getUnitTolerance(); }
    template<>
    inline bool operator==(const Point2D<double>& lhs, const Point2D<double>& rhs) { return std::fabs(lhs.x - rhs.x) < (double)getUnitTolerance() && std::fabs(lhs.y - rhs.y) < (double)getUnitTolerance(); }

    // Point3D related:

    template<>
    constexpr FPoint3D minPoint<FPoint3D>() { return FPoint3D(std::numeric_limits<FPoint3D::coord>::min(), std::numeric_limits<FPoint3D::coord>::min(), std::numeric_limits<FPoint3D::coord>::min()); }

    template<>
    constexpr FPoint3D maxPoint<FPoint3D>() { return FPoint3D(std::numeric_limits<FPoint3D::coord>::max(), std::numeric_limits<FPoint3D::coord>::max(), std::numeric_limits<FPoint3D::coord>::max()); }

    template<typename T>
    constexpr Point3D<T> operator-(const Point3D<T>& p) { return Point3D<T>(-p.x, -p.y, -p.z); }

    template<typename T>
    constexpr Point3D<T> operator+(const Point3D<T>& p, const Point3D<T>& p2) { return Point3D<T>(p.x + p2.x, p.y + p2.y, p.z + p2.z); }

    template<typename T>
    constexpr Point3D<T> operator-(const Point3D<T>& p, const Point3D<T>& p2) { return Point3D<T>(p.x - p2.x, p.y - p2.y, p.z - p2.z); }

    template<typename T>
    constexpr Point3D<T>& operator+=(Point3D<T>& p, const Point3D<T>& p2) { p.x += p2.x; p.y += p2.y; p.z += p2.z; return p; }

    template<typename T>
    constexpr Point3D<T>& operator-=(Point3D<T>& p, const Point3D<T>& p2) { p.x -= p2.x; p.y -= p2.y; p.z -= p2.z; return p; }

    template<typename T, typename V>
    constexpr Point3D<T>  operator*(const Point3D<T>& p, const V val) { return Point3D<T>(p.x * val, p.y * val, p.z * val); }

    template<typename T, typename V>
    constexpr Point3D<T>  operator*(const V i, const Point3D<T>& p) { return p * i; }

    template<typename T>
    constexpr bool operator==(const Point3D<T>& lhs, const Point3D<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }

    template<typename T>
    constexpr bool operator!=(const Point3D<T>& lhs, const Point3D<T>& rhs) { return !(lhs == rhs); }

    // special treatment for float-point number
    template<>
    inline bool operator==(const Point3D<float>& lhs, const Point3D<float>& rhs) { return std::fabs(lhs.x - rhs.x) < getUnitTolerance() && std::fabs(lhs.y - rhs.y) < getUnitTolerance() && std::fabs(lhs.z - rhs.z) < getUnitTolerance(); }
    template<>
    inline bool operator==(const Point3D<double>& lhs, const Point3D<double>& rhs) { return std::fabs(lhs.x - rhs.x) < (double)getUnitTolerance() && std::fabs(lhs.y - rhs.y) < (double)getUnitTolerance() && std::fabs(lhs.z - rhs.z) < (double)getUnitTolerance(); }

} // namespace geom_utils