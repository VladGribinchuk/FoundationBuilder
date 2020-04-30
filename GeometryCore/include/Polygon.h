#pragma once

#include "Point.h"

#include <vector>

namespace geom_utils
{
    struct Triangle2D;

    // Closed 2D polyline
    class Polygon
    {
        // Polygon class should not contain any more additional data fields, only points.
        std::vector<FPoint2D> points;
    public:
        Polygon() = default;

        Polygon(std::initializer_list<FPoint2D> points)
            : points(points)
        {}

        Polygon(const Polygon& other)
            : points(other.points)
        {}

        Polygon& operator=(const Polygon& other)
        {
            points = other.points;
        }

        Polygon(Polygon&& other) noexcept
            : points(std::move(other.points))
        {}

        Polygon& operator=(Polygon&& other) noexcept
        {
            points = std::move(other.points);
        }

        void add(const FPoint2D& p) { points.push_back(p); }
        void remove(unsigned index) { points.erase(points.begin() + index); }
        void clear() { points.clear(); }

        const FPoint2D& operator[] (unsigned index) const { return points[index]; }
        FPoint2D& operator[] (unsigned index) { return points[index]; }

        std::size_t size() const { return points.size(); }
        bool empty() const { return points.empty(); }

        std::vector<FPoint2D>::const_iterator begin() const { return points.begin(); }
        std::vector<FPoint2D>::iterator begin() { return points.begin(); }
        std::vector<FPoint2D>::const_iterator end() const { return points.end(); }
        std::vector<FPoint2D>::iterator end() { return points.end(); }

        // Return length of polyline, note that last and first point are considered as connected
        FPoint2D::coord polygonLength() const;

        // Calculate area of polygon
        FPoint2D::coord area() const;

        // Return true if polygon orientation is counter-clockwise (i.e. area is positive)
        bool orientation() const;

        // Return center of mass of the polygon
        FPoint2D centroid() const;

        // Return closest point in the polygon vertices to the given point p
        FPoint2D closestTo(const FPoint2D& p) const;

        // Translate polygon to the given point
        void translate(const FPoint2D& p);

        // Remove line segments which are smaller than the provided smallestLineLength value.
        // Result will recall smth like re-connecting points process.
        void simplify(const FPoint2D::coord smallestLineLength);

        // Calculate and return convex hull for the polygon
        Polygon convexHull() const;

        // Return true if polygon contour is convex, false if non-convex
        bool isConvexHull() const;

        // The most simple triangulation approach. Work only for convex hull polygons. 
        // For non-convex polygons it should return just empty vector.
        std::vector<Triangle2D> triangulate() const;

        static Polygon makePolygon(const Triangle2D& tri);
    };

}