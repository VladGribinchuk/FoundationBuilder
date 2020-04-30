#include "../include/Polygon.h"
#include "../include/Triangle.h"
#include "../include/GeometryCoreExecEnvironment.h"

namespace geom_utils
{
    bool operator==(const Polygon& lhs, const Polygon& rhs) 
    { 
        return {}; 
    }

    bool operator!=(const Polygon& lhs, const Polygon& rhs)
    {
        return {};
    }

    // Return length of polyline, note that last and first point are considered as connected
    FPoint2D::coord Polygon::polygonLength() const 
    { 
        return {}; 
    }

    // Calculate area of polygon
    inline FPoint2D::coord Polygon::area() const 
    { 
        return {}; 
    }

    // Return true if polygon orientation is counter-clockwise (i.e. area is positive)
    bool Polygon::orientation() const
    { 
        return {}; 
    }

    // Return center of mass of the polygon
    FPoint2D Polygon::centroid() const 
    { 
        return {}; 
    }

    // Return closest point in the polygon vertices to the given point p
    FPoint2D Polygon::closestTo(const FPoint2D& p) const 
    { 
        return {}; 
    }

    // Translate polygon to the given point
    void Polygon::translate(const FPoint2D& p) 
    {
    }


    // Remove line segments which are smaller than the provided smallestLineLength value.
    // Result will recall smth like re-connecting points process.
    void Polygon::simplify(const FPoint2D::coord smallestLineLength) 
    {
    }

    // Calculate and return convex hull for the polygon
    Polygon Polygon::convexHull() const 
    { 
        return {}; 
    }

    // Return true if polygon contour is convex, false if non-convex
    bool Polygon::isConvexHull() const
    { 
        return {}; 
    }

    // The most simple triangulation approach. Work only for convex hull polygons. 
    // For non-convex polygons it should return just empty vector.
    std::vector<Triangle2D> Polygon::triangulate() const
    {
        return {}; 
    }

    Polygon Polygon::makePolygon(const Triangle2D& tri) 
    { 
        Polygon poly; poly.add(tri.a); poly.add(tri.b); poly.add(tri.c); 
        return poly;
    }

}