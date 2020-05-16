#pragma once

#include "Point.h"
#include <cmath>
#include <tuple>

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
        return p.x * p2.x + p.y * p2.y + p.z * p2.z;
    }

    template<typename T>
    constexpr typename Point3D<T> cross(const Point3D<T>& p, const Point3D<T>& p2)
    {
        return Point3D<T>(p.y * p2.z - p.z * p2.y, p.z * p2.x - p.x * p2.z, p.x * p2.y - p.y * p2.x);
    }

    template<typename T>
    inline typename Point3D<T>::coord distance(const Point3D<T>& p, const Point3D<T>& other)
    {
        auto diff = other - p;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    struct LineSegment2D;
    class Polygon;

    //Return projection of point to line segment
    FPoint2D pointProjectionToLineSegment(const FPoint2D& p, const LineSegment2D& line);

    //Return point where lines are intersected
    FPoint2D lineSegmentsIntersection(const LineSegment2D& line1, const LineSegment2D& line2);

    // Return true if line1 is colliding with line2
    bool lineSegmentsCollide(const LineSegment2D& line1, const LineSegment2D& line2);

    // Return distance from the p to the line
    FPoint2D::coord distanceFromPointToLine(const FPoint2D& p, const LineSegment2D& line);
    
    FPoint2D::coord distanceBetweenLineSegments(const LineSegment2D& line1, const LineSegment2D& line2);

    // Return true if poly and line are colliding with each other.
    // Note: consider collision even if one of line end lies exactly on the polygon contour
    bool polygonAndLineSegmentCollides(const Polygon& poly, const LineSegment2D& line);

    //Return true if polygon is close to line (closer than given gap)
    bool polygonAndLineSegmentAreAdjacent(const Polygon& poly, const LineSegment2D& line, const FPoint2D::coord gap);

    // Return true if poly and poly2 are close to each other (closer than the provided gap).
    // Note: test vertices of one polygon to see if they are closer than the gap to any lines in other poly.
    bool polygonsAreAdjacent(const Polygon& poly, const Polygon& poly2, const FPoint2D::coord gap);

} // namespace geom_utils
