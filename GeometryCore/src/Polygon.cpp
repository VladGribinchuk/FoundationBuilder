#include "../include/Polygon.h"
#include "../include/Triangle.h"

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
        //if (this->points.size() < 3) return false;
        float res = 0.0;

        #pragma omp parallel for
        for (int i = 0; i < points.size(); i++)
        {
            FPoint2D v1 = points[i];
            FPoint2D v2 = points[(i + 1) % points.size()];
            #pragma omp atomic
            res += (v2.x - v1.x) * (v2.y + v1.y);
        }

        return res < 0 ? true : false;
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
        //if (points.size() < 3) return; //can't bild a polygon from 2 points or less //idk how to do it
        Polygon convexHull;
        int pos_of_the_most_right_point = 0;

        for (int i = 1; i < points.size(); i++) if (points[i].x < points[pos_of_the_most_right_point].x) pos_of_the_most_right_point = i; //idk how to do it with stl

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
        if (points.size() < 3) return false;
        int convex_flag = 0;
        bool isConvex = true;

        #pragma omp parallel for
        for (int i = 0; i < points.size(); i++)
        {
            FPoint2D v1 = points[i];
            FPoint2D v2 = points[(i + 1) % points.size()];
            FPoint2D v3 = points[(i + 2) % points.size()];
            float res = (v2.x - v1.x) * (v3.y - v2.y) - (v2.y - v1.y) * (v3.x - v2.x);

            #pragma omp critical
            {
                if (res < 0) convex_flag |= 1;
                else if (res > 0) convex_flag |= 2;
                if (convex_flag == 3) isConvex = false;
            }
        }
        if (convex_flag != 0 && isConvex != false) return true;
        else return false;
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