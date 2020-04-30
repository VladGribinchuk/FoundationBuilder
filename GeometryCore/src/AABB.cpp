#include "../include/AABB.h"
#include "../include/Polygon.h"
#include "../include/Triangle.h"

namespace geom_utils
{
    AABB2D::AABB2D(const Polygon& poly)
    {
        calculate(poly);
    }

    AABB2D::AABB2D(const Triangle2D& tri)
    {
        calculate(Polygon::makePolygon(tri));
    }

    bool AABB2D::contains(const FPoint2D& p) const
    { 
        return{}; 
    }

    void AABB2D::calculate(const Polygon& poly)
    {

    }

    bool AABB3D::contains(const FPoint3D& p) const
    {
        return {};
    }
}