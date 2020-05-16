#include "../include/LinearAlgebra.h"
#include "../include/LineSegment.h"
#include "../include/Polygon.h"

namespace geom_utils
{
    //Return projection of point to line segment
    FPoint2D pointProjectionToLineSegment(const FPoint2D& p, const LineSegment2D& line) {
        auto ortogonal_vector_x = line.b.y - line.a.y;
        auto ortogonal_vector_y = line.a.x - line.b.x;
        auto L = (cross(line.a, line.b) + cross(p, line.a) + cross(line.b, p)) / (ortogonal_vector_x * ortogonal_vector_x + ortogonal_vector_y * ortogonal_vector_y);
        FPoint2D projection;
        projection.x = p.x + ortogonal_vector_x * L;
        projection.y = p.y + ortogonal_vector_y * L;
        return projection;
    }

    //Return point where lines are intersected
    //If lines are parallel result of it is infinity
    FPoint2D lineSegmentsIntersection(const LineSegment2D& line1, const LineSegment2D& line2) {
        using namespace std;
        using standard_line_view = tuple<FPoint2D::coord, FPoint2D::coord, FPoint2D::coord>;
        auto to_standard_line_view = [](const LineSegment2D& l) {return make_tuple(l.a.y - l.b.y, l.b.x - l.a.x, cross(l.a, l.b)); };

        standard_line_view stline1 = to_standard_line_view(line1);
        standard_line_view stline2 = to_standard_line_view(line2);

        FPoint2D intersection;
        intersection.x = (get<2>(stline2) * get<1>(stline1) - get<2>(stline1) * get<1>(stline2)) / (get<0>(stline1) * get<1>(stline2) - get<0>(stline2) * get<1>(stline1));
        intersection.y = (get<2>(stline1) * get<0>(stline2) - get<2>(stline2) * get<0>(stline1)) / (get<1>(stline2) * get<0>(stline1) - get<1>(stline1) * get<0>(stline2));
        return intersection;
    }

    // Return true if line1 is colliding with line2
    bool lineSegmentsCollide(const LineSegment2D& line1, const LineSegment2D& line2) {
        auto intersection = lineSegmentsIntersection(line1, line2);
        if ((intersection.x >= std::min(line1.a.x, line1.b.x)) && (intersection.x <= std::max(line1.a.x, line1.b.x)) &&
            (intersection.x >= std::min(line2.a.x, line2.b.x)) && (intersection.x <= std::max(line2.a.x, line2.b.x)) &&
            (intersection.y >= std::min(line1.a.y, line1.b.y)) && (intersection.y <= std::max(line1.a.y, line1.b.y)) &&
            (intersection.y >= std::min(line2.a.y, line2.b.y)) && (intersection.y <= std::max(line2.a.y, line2.b.y)))
        {
            return true;
        }
        return false;
    }

    // Return distance from the p to the line
    FPoint2D::coord distanceFromPointToLine(const FPoint2D& p, const LineSegment2D& line) {
        auto projection = pointProjectionToLineSegment(p, line);

        if ((projection.x >= std::min(line.a.x, line.b.x)) && (projection.x <= std::max(line.a.x, line.b.x)) &&
            (projection.y >= std::min(line.a.y, line.b.y)) && (projection.y <= std::max(line.a.y, line.b.y))) {
            return distance(projection, p);
        }
        else {
            return std::min(distance(line.a, p), distance(line.b, p));
        }
    }

    FPoint2D::coord distanceBetweenLineSegments(const LineSegment2D& line1, const LineSegment2D& line2) {
        auto check_projection = [](const FPoint2D& p, const LineSegment2D& line, FPoint2D::coord& minimal_distance) { minimal_distance = std::min(distanceFromPointToLine(p, line), minimal_distance); };

        FPoint2D::coord min_dis = std::min(std::min(distance(line1.a, line2.a), distance(line1.a, line2.b)), std::min(distance(line1.b, line2.a), distance(line1.b, line2.b)));
        check_projection(line2.a, line1, min_dis);
        check_projection(line2.b, line1, min_dis);
        check_projection(line1.a, line2, min_dis);
        check_projection(line1.b, line2, min_dis);

        return min_dis;
    }

    // Return true if poly and line are colliding with each other.
    // Note: consider collision even if one of line end lies exactly on the polygon contour
    bool polygonAndLineSegmentCollides(const Polygon& poly, const LineSegment2D& line) {
        int size = poly.size();
        for (int i = 0; i < size; ++i) {
            LineSegment2D side(poly[i], poly[(i + 1) % size]);
            if (lineSegmentsCollide(line, side)) {
                return true;
            }
        }
        return false;
    }

    //Return true if polygon is close to line (closer than given gap)
    bool polygonAndLineSegmentAreAdjacent(const Polygon& poly, const LineSegment2D& line, const FPoint2D::coord gap) {
        int size = poly.size();
        if (size == 0) {
            return false;
        }

        std::vector<FPoint2D::coord> distance_from_line_to_polygon_side(size);
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            LineSegment2D side(poly[i], poly[(i + 1) % size]);
            distance_from_line_to_polygon_side[i] = distanceBetweenLineSegments(line, side);
        }
        return *std::min_element(distance_from_line_to_polygon_side.begin(), distance_from_line_to_polygon_side.end()) <= gap;
    }

    // Return true if poly and poly2 are close to each other (closer than the provided gap).
    // Note: test vertices of one polygon to see if they are closer than the gap to any lines in other poly.
    bool polygonsAreAdjacent(const Polygon& poly, const Polygon& poly2, const FPoint2D::coord gap) {
        int size = poly2.size();
        if (size == 0) {
            return false;
        }
        std::vector<bool> polygon_side_closer_to_second_polygon_than_gap(size);
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
                LineSegment2D side(poly2[i], poly2[(i + 1) % size]);
                polygon_side_closer_to_second_polygon_than_gap[i] = polygonAndLineSegmentAreAdjacent(poly, side, gap);
        }
        return std::any_of(polygon_side_closer_to_second_polygon_than_gap.begin(), polygon_side_closer_to_second_polygon_than_gap.end(), [](bool k) {return k; });
    }
}