#include "../include/AABB.h"
#include "../include/Polygon.h"
#include "../include/Triangle.h"
#include "../include/Mesh.h"

namespace geom_utils
{
    AABB2D::AABB2D(const Polygon& poly)
    {
        calculate(poly);
    }

    AABB3D::AABB3D(const Mesh& figure)
    {
        calculate(figure);
    }

    AABB2D::AABB2D(const Triangle2D& tri)
    {
        calculate(Polygon::makePolygon(tri));
    }

    bool AABB2D::isValid() 
    {
        if (pmax.x < pmin.x || pmax.y < pmin.y) return false;
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
        else 
        {
            pmin = (maxPoint<FPoint2D>());
            pmax = (minPoint<FPoint2D>());
        }
        
    }

    void AABB3D::calculate(const Mesh& figure)
    { 
        if (!figure.isEmpty()) {
            std::vector <Triangle3D> facets(figure.getFacets());
            std::vector<FPoint3D::coord> valuesX;
            std::vector<FPoint3D::coord> valuesY;
            std::vector<FPoint3D::coord> valuesZ;
            for (auto i : facets)
            {
                valuesX.push_back(i.a.x);
                valuesX.push_back(i.b.y);
                valuesX.push_back(i.c.z);

                valuesY.push_back(i.a.x);
                valuesY.push_back(i.b.y);
                valuesY.push_back(i.c.z);

                valuesZ.push_back(i.a.z);
                valuesZ.push_back(i.b.z);
                valuesZ.push_back(i.c.z);
            }

            pmin.x = *std::min_element(valuesX.begin(), valuesX.end());
            pmax.x = *std::max_element(valuesX.begin(), valuesX.end());

            pmin.y = *std::min_element(valuesY.begin(), valuesY.end());
            pmax.y = *std::max_element(valuesY.begin(), valuesY.end());

            pmin.z = *std::min_element(valuesZ.begin(), valuesZ.end());
            pmax.z = *std::max_element(valuesZ.begin(), valuesZ.end());
     
        }
        else
        {
            pmin = (maxPoint<FPoint3D>());
            pmax = (minPoint<FPoint3D>());
        }

    }

    bool AABB3D::contains(const FPoint3D& p) const
    {
        if (p.x <= pmax.x && p.y <= pmax.y && p.z <= pmax.z && p.x >= pmin.x && p.y >= pmin.y && p.z >= pmin.z)  return true;
        else return false;
    }
}