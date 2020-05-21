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

    bool AABB2D::isValid(const AABB2D& aabb2d) 
    {
        if (aabb2d.pmax.x <= aabb2d.pmin.x || aabb2d.pmax.y <= aabb2d.pmin.y) return false;
        else return true;
    }

    bool AABB2D::contains(const FPoint2D& p) const
    {
        if (p.x <= pmax.x && p.y <= pmax.y && p.x >= pmin.x && p.y >= pmin.y)  return true;
        else return false;
    }

    void AABB2D::calculate(const Polygon& poly)
    {
        if (!poly.empty()) {
            auto maxX = std::max_element(poly.begin(), poly.end(), [](auto a, auto b) {return b.x > a.x; }) - poly.begin();
            pmax.x = poly[maxX].x;
            auto maxY = std::max_element(poly.begin(), poly.end(), [](auto a, auto b) {return b.y > a.y; }) - poly.begin();
            pmax.y = poly[maxY].y;
            auto minX = std::min_element(poly.begin(), poly.end(), [](auto a, auto b) {return b.x > a.x; }) - poly.begin();
            pmin.x = poly[minX].x;
            auto minY = std::min_element(poly.begin(), poly.end(), [](auto a, auto b) {return b.y > a.y; }) - poly.begin();
            pmin.y = poly[minY].y;
        }
        
    }

    bool AABB3D::contains(const FPoint3D& p) const
    {
        if (p.x <= pmax.x && p.y <= pmax.y && p.z <= pmax.z && p.x >= pmin.x && p.y >= pmin.y && p.z >= pmin.z)  return true;
        else return false;
    }
}