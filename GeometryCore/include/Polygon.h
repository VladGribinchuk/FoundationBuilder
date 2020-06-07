#pragma once

#include "Point.h"
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>

namespace geom_utils
{
    struct Triangle2D;

    // Closed 2D polyline
    class Polygon
    {
        // Polygon class should not contain any more additional data fields, only points.
        std::vector<FPoint2D> points;
    public:
        typedef FPoint2D PointType;
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
            return *this;
        }

        Polygon(Polygon&& other) noexcept
            : points(std::move(other.points))
        {}

        Polygon& operator=(Polygon&& other) noexcept
        {
            points = std::move(other.points);
            return *this;
        }

        void add(const FPoint2D& p) { points.push_back(p); }
        void addRange(const std::vector<FPoint2D>& p) { std::copy(p.begin(), p.end(), std::back_inserter(points)); }
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

        //simplifies polygon based on Ramer–Douglas–Peucker algorithm
        void simplify(const FPoint2D::coord epsilon);

        // Calculate and return convex hull for the polygon
        Polygon convexHull() const;

        // Return true if polygon contour is convex, false if non-convex
        bool isConvexHull() const;

        // The most simple triangulation approach. Work only for convex hull polygons. 
        // For non-convex polygons it should return just empty vector.
        std::vector<Triangle2D> triangulate() const;

        static Polygon makePolygon(const Triangle2D& tri);

        //return inflate poligon
        Polygon inflate(const float value) const;
       
    private:
        // Builds a new curve with fever points
        //The algorithm defines 'dissimilar' based on the maximum distance between the original curve and the simplified curve (i.e., the Hausdorff distance between the curves)
        //Link - https://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
        void simplifyRamerDouglasPeucker(const std::vector<FPoint2D>& pointList, const FPoint2D::coord epsilon, std::vector<FPoint2D>& out);

    };

    bool operator==(const Polygon& lhs, const Polygon& rhs);
    bool operator!=(const Polygon& lhs, const Polygon& rhs);
    // Polygon output/input operators

    inline std::string PolygonToString(const Polygon& poly)
    {
        std::stringstream ss;
        ss << "{ ";
        if (!poly.empty())
        {
            auto it = poly.begin();
            for (; it < poly.end(); ++it)
                ss << *it << (it == std::prev(poly.end()) ? " " : "; ");
        }
        ss << "}";
        return ss.str();
    }

    inline std::ostream& operator<<(std::ostream& os, const Polygon& poly)
    {
        os << PolygonToString(poly);
        return os;
    }

    inline std::istream& operator>>(std::istream& is, Polygon& poly)
    {
        poly.clear();

        char c;
        while (is.get(c) && c != '{') {/*search for opening brace*/}

        Polygon::PointType point;
        while (is)
        {
            is >> point;
            if (point != notAPoint<Polygon::PointType>())
                poly.add(point);
            while (is.get(c) && c != ';' && c != '}') { /*search for next point*/ }
            if (c == '}') // closing brace was found, exit
                break;
        }
        return is;
    }

}