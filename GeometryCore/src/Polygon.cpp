#include "../include/Polygon.h"
#include "../include/Triangle.h"
#include "../include/LinearAlgebra.h"

namespace geom_utils
{
    bool operator==(const Polygon& lhs, const Polygon& rhs) 
    { 
        if (lhs.size() != rhs.size()) {
            return false;
        }
        if (lhs.empty() && rhs.empty()) {
            return true;
        }
        std::vector<FPoint2D>::const_iterator lhs_temp_point = lhs.begin();
        auto rhs_temp_point = std::find(rhs.begin(), rhs.end(), *lhs_temp_point);
        if (rhs_temp_point == rhs.end()) {
            return false;
        }
        for (; lhs_temp_point != lhs.end(); ++lhs_temp_point, ++rhs_temp_point) {
            if (rhs_temp_point == rhs.end()) {
                rhs_temp_point = rhs.begin();
            }
            if (*lhs_temp_point != *rhs_temp_point) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Polygon& lhs, const Polygon& rhs)
    {
        return !(lhs == rhs);
    }

    // Return length of polyline, note that last and first point are considered as connected
    FPoint2D::coord Polygon::polygonLength() const 
    {
        int size = points.size();
        if (size < 2) {
            return 0;
        }
        else if (size == 2) {
            return distance(points[0], points[1]);
        }
        FPoint2D::coord length = 0;
        for (int i = 0; i < size; ++i) {
            length += distance(points[i], points[(i + 1) % size]);
        }
        return length;
    }

    // Calculate area of polygon
    FPoint2D::coord Polygon::area() const 
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
        int size = points.size();
        if (size == 0) {
            return FPoint2D();
        }
        FPoint2D center_of_mass(FPoint2D::coord(0.00), FPoint2D::coord(0.00));
        FPoint2D::coord det = 0, temp_det = 0;
        int j = 0;
        for (int i = 0; i < size; ++i){
            j = (i + 1) % size;
            temp_det = cross(points[i], points[j]);
            det += temp_det;
            center_of_mass.x += (points[i].x + points[j].x) * temp_det;
            center_of_mass.y += (points[i].y + points[j].y) * temp_det;
        }
        FPoint2D::coord full_polygon_mass = 3 * det;
        center_of_mass.x /= full_polygon_mass;
        center_of_mass.y /= full_polygon_mass;

        return center_of_mass;
    }

    // Return closest point in the polygon vertices to the given point p
    FPoint2D Polygon::closestTo(const FPoint2D& p) const 
    {
        int size = points.size();
        if (size == 0) {
            return FPoint2D();
        }
        std::vector<FPoint2D::coord> distances(size);
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            distances[i] = distance(points[i], p);
        }
        return points[std::distance(distances.begin(), min_element(distances.begin(), distances.end()))];
    }

    // Translate polygon to the given point
    void Polygon::translate(const FPoint2D& p) 
    {
        std::for_each(points.begin(), points.end(), [&](FPoint2D& n) { n += p; });
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

        auto lexicographicallyPred = [](const FPoint2D& a, const FPoint2D& b) {return a.x < b.x || (a.x == b.x && a.y < b.y); };
        size_t k = 0;
        Polygon convexHull;
        convexHull.points.resize(2 * this->points.size());

        // Sort points lexicographically
        auto pointsCopy = this->points;
        std::sort(pointsCopy.begin(), pointsCopy.end(), lexicographicallyPred);

        // Build lower hull
        for (size_t i = 0; i < pointsCopy.size(); ++i) {
            while (k >= 2 && geom_utils::cross(FPoint2D(convexHull.points[k - 2] - pointsCopy[i]), FPoint2D(convexHull.points[k - 1] - pointsCopy[i])) <= 0) k--;
            convexHull.points[k++] = pointsCopy[i];
        }

        // Build upper hull
        for (size_t i = pointsCopy.size() - 1, t = k + 1; i > 0; --i) {
            while (k >= t && geom_utils::cross(FPoint2D(convexHull.points[k - 2] - pointsCopy[i - 1]), FPoint2D(convexHull.points[k - 1] - pointsCopy[i - 1])) <= 0) k--;
            convexHull.points[k++] = pointsCopy[i - 1];
        }
        convexHull.points.resize(k - 1);

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