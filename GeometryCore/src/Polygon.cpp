#include "../include/Polygon.h"
#include "../include/Triangle.h"
#include "../include/LinearAlgebra.h"

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
        if (this->points.size() < 3) return 0; //can't measure an area of a line or point
        float res = 0.0;

        #pragma omp parallel for
        for (int i = 0; i < points.size(); i++)
        {
            FPoint2D v1 = points[i];
            FPoint2D v2 = points[(i + 1) % points.size()];
            #pragma omp atomic
            res += (v2.x - v1.x) * (v2.y + v1.y);
        }

        return res/2;
    }

    // Return true if polygon orientation is counter-clockwise (i.e. area is positive)
    bool Polygon::orientation() const
    {
        return area() < 0;
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
        if (points.size() <= 3) return *this; //can't bild a polygon from 2 points or less
        Polygon convexHull;
        auto pos_of_the_most_right_point = std::min_element(points.begin(), points.end(), [](auto a, auto b) {return b.x > a.x; }) - points.begin();
        int pos_of_cur_point = pos_of_the_most_right_point;

        do
        {
            convexHull.points.push_back(points[pos_of_cur_point]);
            int pos_of_next_point = (pos_of_cur_point + 1) % points.size();

            for (int i = 0; i < points.size(); i++)
            {   //find the most counter-clockwise point
                Polygon tmp_poly({ points[pos_of_cur_point], points[i], points[pos_of_next_point] });
                if (tmp_poly.orientation())
                    pos_of_next_point = i;
            }
            pos_of_cur_point = pos_of_next_point;

        } while (pos_of_cur_point != pos_of_the_most_right_point);

        return convexHull;
    }

    // Return true if polygon contour is convex, false if non-convex
    bool Polygon::isConvexHull() const
    {
        if (points.size() < 2) return false;

        bool isCounterClockwise = orientation();
        for (int i = 0; i < points.size(); i++)
        {
            auto vec1 = points[(i + 1) % points.size()] - points[i];
            auto vec2 = points[(i + 2) % points.size()] - points[i];

            if (isCounterClockwise ? cross(vec1, vec2) < 0 : cross(vec1, vec2) > 0)
                return false;
        }
        return true;
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