#include "../include/Polygon.h"
#include "../include/Triangle.h"
#include "../include/LinearAlgebra.h"
#include "../include/LineSegment.h"



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

    // Builds a new curve with fever points
    //The algorithm defines 'dissimilar' based on the maximum distance between the original curve and the simplified curve (i.e., the Hausdorff distance between the curves)
    //Link - https://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
    void Polygon::simplifyRamerDouglasPeucker(const std::vector<FPoint2D>& pointList, const FPoint2D::coord epsilon, std::vector<FPoint2D>& out)
    {
            float dmax = 0.0;
            int index = 0;
            int end = pointList.size() - 1;

            for (int i = 1; i < pointList.size() - 1; i++)
            {
                LineSegment2D line(pointList[0], pointList[end]);
                float d = distanceFromPointToLine(pointList[i], line);
                if (d > dmax)
                {
                    index = i;
                    dmax = d;
                }

            }
            if (dmax > epsilon)
            {
               // Recursive call
               std:: vector<FPoint2D> recResults1;
               std:: vector<FPoint2D> recResults2;
               std:: vector<FPoint2D> firstLine(pointList.begin(), pointList.begin() + index + 1);
               std::vector<FPoint2D> lastLine(pointList.begin() + index, pointList.end());
               simplifyRamerDouglasPeucker(firstLine, epsilon, recResults1);
               simplifyRamerDouglasPeucker(lastLine, epsilon, recResults2);
               // Build the result list
               out.assign(recResults1.begin(), recResults1.end() - 1);
               out.insert(out.end(), recResults2.begin(), recResults2.end());

            }
            else
            {
                //Just return start and end points
                out.clear();
                out.push_back(pointList[0]);
                out.push_back(pointList[end]);

            }
   }

    //simplifies polygon based on Ramer�Douglas�Peucker algorithm
    void Polygon::simplify(const FPoint2D::coord epsilon)
    {
        if (points.size() > 2) 
        {
            std::vector<FPoint2D> out;
            simplifyRamerDouglasPeucker(points, epsilon, out);
            if (out.size() > 2)
            {
              points.swap(out);
              out.clear();
            }
        }
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
            while (k >= 2 && geom_utils::cross(FPoint2D(convexHull.points[k - 1] - convexHull.points[k - 2]), FPoint2D(pointsCopy[i] - convexHull.points[k - 2])) <= 0) k--;
            convexHull.points[k++] = pointsCopy[i];
        }

        // Build upper hull
        for (size_t i = pointsCopy.size() - 1, t = k + 1; i > 0; --i) {
            while (k >= t && geom_utils::cross(FPoint2D(convexHull.points[k - 1] - convexHull.points[k - 2]), FPoint2D(pointsCopy[i - 1] - convexHull.points[k - 2])) <= 0) k--;
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
        std::vector <Triangle2D> vectorForTriangulate;
        if (isConvexHull())
            {
                FPoint2D  firstVertex =  points[0];
                for (int i = 0; i < points.size() - 2 ; i++)
                {
                   
                    Triangle2D tri = {points[(i + 2) % points.size()] ,firstVertex, points[(i+1) % points.size()] };
                    vectorForTriangulate.push_back(tri);
                }
            }         
        return vectorForTriangulate;
    }


    Polygon Polygon::makePolygon(const Triangle2D& tri) 
    { 
        Polygon poly; poly.add(tri.a); poly.add(tri.b); poly.add(tri.c); 
        return poly;
    }

    Polygon Polygon::inflate(const float value, const float cornerAllowanceMultiplier) const
    {
        // hold 2x more points than original
        // to enable concurrent access from parallel loop
        Polygon poly;
        poly.points.resize(this->size() * 2, notAPoint<FPoint2D>());

        bool isCCW = orientation();

        #pragma omp parallel for
        for (int i = 0; i < points.size(); i++) 
        {
            // get this point (pt1), the point before it (pt0) and the point that follows it (pt2)
            FPoint2D point0 = points[(i > 0) ? i - 1 : points.size() - 1];
            FPoint2D point1 = points[i];
            FPoint2D point2 = points[(i + 1) % points.size()];

            // find the line vectors of the lines going to the current point
            FPoint2D v1(point1 - point0);
            FPoint2D v2(point2 - point1);

            FPoint2D rotPoint1 = (!isCCW ? vecRot90CCW(v1) : vecRot90CW(v1));
            FPoint2D rotPoint2 = (!isCCW ? vecRot90CCW(v2) : vecRot90CW(v2));

            // find the normals of the two lines, multiplied to the distance that polygon should inflate
            FPoint2D offsetVec1 = normalizeVector(rotPoint1) * value;
            FPoint2D offsetVec2 = normalizeVector(rotPoint2) * value;

            if (std::fabs(cross(v1, v2)) < getUnitTolerance())
            { // original lines are collinear
                auto dist = distance(point1 + offsetVec1, point1 + offsetVec2);
                if (dist > getUnitTolerance())
                {  // sides are not close to each other, so most likely they are shaping 0 degree angle
                    poly.points[i * 2] = point1 + offsetVec1;
                    poly.points[i * 2 + 1] = point1 + offsetVec2;
                }
            }
            else
            { // lines are not collinear, so can find intersection safely
                LineSegment2D inflatedSide1(point0 + offsetVec1, point1 + offsetVec1);
                LineSegment2D inflatedSide2(point1 + offsetVec2, point2 + offsetVec2);
                auto intersection = lineSegmentsIntersection(inflatedSide1, inflatedSide2);

                auto distanceFromOriginalCornerToInflated = distance(point1, intersection);
                if (distanceFromOriginalCornerToInflated <= value * cornerAllowanceMultiplier)
                {
                    poly.points[i * 2] = intersection;
                }
                else
                { // angle is too sharp for the provided threshold, so clip it
                    auto corningVector = normalizeVector(intersection - point1) * (value * cornerAllowanceMultiplier);
                    poly.points[i * 2] = pointProjectionToLineSegment(point1 + corningVector, inflatedSide1);
                    poly.points[i * 2 + 1] = pointProjectionToLineSegment(point1 + corningVector, inflatedSide2);
                }
            }
        }

        poly.points.erase(std::remove(poly.points.begin(), poly.points.end(), notAPoint<FPoint2D>()), poly.points.end());

        return poly;
    }
}