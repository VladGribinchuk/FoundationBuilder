#pragma once

#include "Point.h"

namespace geom_utils
{
    class Polygon;
    struct Triangle2D;

    class AABB2D // Axis Aligned Boundary Box
    {
    public:
        FPoint2D pmin, pmax;

        AABB2D()
            : pmin(maxPoint<FPoint2D>()), pmax(minPoint<FPoint2D>())
        {}

        AABB2D(const FPoint2D& min, const FPoint2D& max)
            : pmin(min), pmax(max)
        {}

        AABB2D(const Polygon&);

        AABB2D(const Triangle2D&);

        FPoint2D min() const { return pmin; }
        FPoint2D max() const { return pmax; }

        // Return true if p is inside the AABB boundaries.
        bool contains(const FPoint2D& p) const;

    private:
        // Calculate min, max based on given poly.
        void calculate(const Polygon&);
    };

    class AABB3D // Axis Aligned Boundary Box
    {
    public:
        FPoint3D pmin, pmax;

        AABB3D()
            : pmin(maxPoint<FPoint3D>()), pmax(minPoint<FPoint3D>())
        {}

        AABB3D(const FPoint3D& min, const FPoint3D& max)
            : pmin(min), pmax(max)
        {}

        FPoint3D min() const { return pmin; }
        FPoint3D max() const { return pmax; }

        // Return true if p is inside the AABB boundaries.
        bool contains(const FPoint3D& p) const;
    };
}
