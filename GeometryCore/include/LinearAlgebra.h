#pragma once

#include "Point.h"
#include <cmath>

namespace geom_utils
{
    // Point2D related:

    template<typename T>
    constexpr typename Point2D<T>::coord dot(const Point2D<T>& p, const Point2D<T>& p2)
    {
        return p.x * p2.x + p.y * p2.y;
    }

    template<typename T>
    constexpr typename Point2D<T>::coord cross(const Point2D<T>& p, const Point2D<T>& p2)
    {
        return p.x * p2.y - p2.x * p.y;
    }

    template<typename T>
    inline typename Point2D<T>::coord distance(const Point2D<T>& p, const Point2D<T>& other)
    {
        auto diff = other - p;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    // Point3D related:

    template<typename T>
    constexpr typename Point3D<T>::coord dot(const Point3D<T>& p, const Point3D<T>& p2)
    {
        static_assert(false, "dot() is not implemented yet for Point3D");
        return {};
    }

    template<typename T>
    constexpr typename Point3D<T>::coord cross(const Point3D<T>& p, const Point3D<T>& p2)
    {
        static_assert(false, "cross() is not implemented yet for Point3D");
        return {};
    }

    template<typename T>
    inline typename Point3D<T>::coord distance(const Point3D<T>& p, const Point3D<T>& other)
    {
        static_assert(false, "cross() is not implemented yet for Point3D");
        return {};
    }

    struct LineSegment2D;
    class Polygon;

    // Return true if line1 is colliding with line2
    bool lineSegmentsCollide(const LineSegment2D& line1, const LineSegment2D& line2);

    // Return distance from the p to the line
    FPoint2D::coord distanceFromPointToLine(const FPoint2D& p, const LineSegment2D& line);

    // Return true if poly and line are colliding with each other.
    // Note: consider collision even if one of line end lies exactly on the polygon contour
    bool polygonAndLineSegmentCollides(const Polygon& poly, const LineSegment2D& line);

    // Return true if poly and poly2 are close to each other (closer than the provided gap).
    // Note: test vertices of one polygon to see if they are closer than the gap to any lines in other poly.
    bool polygonsAreAdjacent(const Polygon& poly, const Polygon& poly2, const FPoint2D::coord gap = 1);

} // namespace geom_utils
