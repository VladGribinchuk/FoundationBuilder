#include "../include/LinearAlgebra.h"
#include "../include/LineSegment.h"
#include "../include/Polygon.h"
#include "../include/GeometryCoreExecEnvironment.h"

namespace geom_utils
{
    // Return true if line1 is colliding with line2
    bool lineSegmentsCollide(const LineSegment2D& line1, const LineSegment2D& line2)
    {
        return {};
    }

    // Return distance from the p to the line
    FPoint2D::coord distanceFromPointToLine(const FPoint2D& p, const LineSegment2D& line)
    {
        return {};
    }

    // Return true if poly and line are colliding with each other.
    // Note: consider collision even if one of line end lies exactly on the polygon contour
    bool polygonAndLineSegmentCollides(const Polygon& poly, const LineSegment2D& line)
    {
        return {};
    }

    // Return true if poly and poly2 are close to each other (closer than the provided gap).
    // Note: test vertices of one polygon to see if they are closer than the gap to any lines in other poly.
    bool polygonsAreAdjacent(const Polygon& poly, const Polygon& poly2, const FPoint2D::coord gap)
    {
        return {};
    }
}