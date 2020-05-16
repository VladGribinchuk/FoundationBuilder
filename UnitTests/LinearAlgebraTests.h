#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/LineSegment.h"
#include "../GeometryCore/include/LinearAlgebra.h"

using namespace geom_utils;

//this segment for 2d points
DEFINE_TEST_CASE(PosPointsDot2D)
{
	FPoint2D first(200, 500);
	FPoint2D second(300, 100);
	TEST_CHECK(dot(first, second), mm(110000.00), "Dot must be 110000 mm!");
}
DEFINE_TEST_CASE(PosPointsCross2D)
{
	FPoint2D first(150, 400);
	FPoint2D second(50, 90);
	TEST_CHECK(cross(first, second), mm(-6500.00), "Cross must be -6500 mm!");
}
DEFINE_TEST_CASE(PosPointsDistance2D)
{
	FPoint2D first(40, 50);
	FPoint2D second(40, 60);
	TEST_CHECK(distance(first, second), mm(10.00), "Distance must be 10 mm!");
}

DEFINE_TEST_CASE(NegPointsDot2D)
{
	FPoint2D first(-150, -250);
	FPoint2D second(-300, -100);
	TEST_CHECK(dot(first, second), mm(70000.00), "Dot must be 70000 mm!");
}
DEFINE_TEST_CASE(NegPointsCross2D)
{
	FPoint2D first(-150, -200);
	FPoint2D second(-50, -90);
	TEST_CHECK(cross(first, second), mm(3500.00), "Cross must be 3500 mm!");
}
DEFINE_TEST_CASE(NegPointsDistance2D)
{
	FPoint2D first(-30, -50);
	FPoint2D second(-30, -60);
	TEST_CHECK(distance(first, second), mm(10.00), "Distance must be 10 mm!");
}

DEFINE_TEST_CASE(MixPointsDot2D)
{
	FPoint2D first(-150, 250);
	FPoint2D second(-300, 100);
	TEST_CHECK(dot(first, second), mm(70000.00), "Dot must be 70000 mm!");
}
DEFINE_TEST_CASE(MixPointsCross2D)
{
	FPoint2D first(150, -400);
	FPoint2D second(50, -90);
	TEST_CHECK(cross(first, second), mm(6500.00), "Cross must be 6500 mm!");
}
DEFINE_TEST_CASE(MixPointsDistance2D)
{
	FPoint2D first(-40, -50);
	FPoint2D second(-40, 60);
	TEST_CHECK(distance(first, second), mm(110.00), "Distance must be 110 mm!");
}


// this segment for 3d points
DEFINE_TEST_CASE(PosPointsDot3D)
{
	FPoint3D first(10, 20, 30);
	FPoint3D second(40, 50, 60);
	TEST_CHECK(dot(first, second), mm(3200.00), "Dot must be 3200 mm!");
}
DEFINE_TEST_CASE(PosPointsCross3D)
{
	FPoint3D first(10, 20, 30);
	FPoint3D second(50, 40, 40);
	TEST_CHECK(cross(first, second), FPoint3D(-400, 1100, -600), "Cross must be 100 mm!");
}
DEFINE_TEST_CASE(PosPointsDistance3D)
{
	FPoint3D first(40, 50, 10);
	FPoint3D second(40, 50, 90);
	TEST_CHECK(distance(first, second), mm(80.00), "Distance must be 80 mm!");
}

DEFINE_TEST_CASE(NegPointsDot3D)
{
	FPoint3D first(-5, -30, -30);
	FPoint3D second(-40, -50, -10);
	TEST_CHECK(dot(first, second), mm(2000.00), "Dot must be 2000 mm!");
}
DEFINE_TEST_CASE(NegPointsCross3D)
{
	FPoint3D first(-10, -5, -20);
	FPoint3D second(-30, -10, -10);
	TEST_CHECK(cross(first, second), FPoint3D(-150, 500, -50), "Cross must be 300 mm!");
}
DEFINE_TEST_CASE(NegPointsDistance3D)
{
	FPoint3D first(-5, -5, -10);
	FPoint3D second(-5, -5, -90);
	TEST_CHECK(distance(first, second), mm(80.00), "Distance must be 80 mm!");
}

DEFINE_TEST_CASE(MixPointsDot3D)
{
	FPoint3D first(-10, 20, 30);
	FPoint3D second(40, -50, 60);
	TEST_CHECK(dot(first, second), mm(400.00), "Dot must be 400 mm!");
}
DEFINE_TEST_CASE(MixPointsCross3D)
{
	FPoint3D first(-5, 10, 30);
	FPoint3D second(40, -10, -20);
	TEST_CHECK(cross(first, second), FPoint3D(100, 1100, -350), "Cross must be 850 mm!");
}
DEFINE_TEST_CASE(MixPointsDistance3D)
{
	FPoint3D first(-40, 60, -10);
	FPoint3D second(-40, 60, 90);
	TEST_CHECK(distance(first, second), mm(100.00), "Distance must be 100 mm!");
}

////
DEFINE_TEST_CASE(PointProjToLine)
{
	FPoint2D p1(5, 8), p2(9, 4), p3(-2, -1), p4(7, 4);
	LineSegment2D L(FPoint2D(6, 5), FPoint2D(9, 2));
	TEST_CHECK(pointProjectionToLineSegment(p1, L), FPoint2D(4, 7), "Point projection for this line must be {4, 7}");
	TEST_CHECK(pointProjectionToLineSegment(p2, L), FPoint2D(8, 3), "Point projection for this line must be {8, 3}");
	TEST_CHECK(pointProjectionToLineSegment(p3, L), FPoint2D(5, 6), "Point projection for this line must be {5, 6}");
	TEST_CHECK(pointProjectionToLineSegment(p4, L), FPoint2D(7, 4), "Point projection for this line must be {7, 4}");
}

DEFINE_TEST_CASE(LinesIntersection)
{
	LineSegment2D L1(FPoint2D(3, 4), FPoint2D(7, 4)), L2(FPoint2D(3, 5), FPoint2D(5, 7)), L3(FPoint2D(7, 3), FPoint2D(3, 7));
	TEST_CHECK(lineSegmentsIntersection(L1, L2), FPoint2D(2, 4), "Lines must intersection in point {2, 4}");
	TEST_CHECK(lineSegmentsIntersection(L1, L3), FPoint2D(6, 4), "Lines must intersection in point {6, 4}");
	TEST_CHECK(lineSegmentsIntersection(L2, L3), FPoint2D(4, 6), "Lines must intersection in point {4, 6}");
}

DEFINE_TEST_CASE(LinesCollide)
{
	LineSegment2D L1(FPoint2D(3, 4), FPoint2D(7, 4)), L2(FPoint2D(3, 5), FPoint2D(5, 7)), L3(FPoint2D(7, 3), FPoint2D(3, 7));
	TEST_CHECK(lineSegmentsCollide(L1, L2), false, "Line segment ({3, 4}, {7, 4}) must not collide with line segment ({3, 5}, {5, 7})");
	TEST_CHECK(lineSegmentsCollide(L1, L3), true, "Line segment ({3, 4}, {7, 4}) must collide with line segment ({7, 3}, {3, 7})");
	TEST_CHECK(lineSegmentsCollide(L2, L3), true, "Line segment ({3, 5}, {5, 7}) must collide with line segment ({7, 3}, {3, 7})");
}

DEFINE_TEST_CASE(DistanceFromPToLine)
{
	FPoint2D p1(6, 8), p2(8, 3);
	LineSegment2D L1(FPoint2D(6, 5), FPoint2D(9, 2)), L2(FPoint2D(6, 5), FPoint2D(6, 2));
	TEST_CHECK(distanceFromPointToLine(p1, L1), mm(3.00), "Distance must be 3 mm");
	TEST_CHECK(distanceFromPointToLine(p2, L2), mm(2.00), "Distance must be 2 mm");
}

DEFINE_TEST_CASE(DistanceBetweenLineSegm)
{
	LineSegment2D L1(FPoint2D(6, 5), FPoint2D(9, 2)), L2(FPoint2D(6, 5), FPoint2D(6, 2)), L3(FPoint2D(4, 2), FPoint2D(5, 5));
	TEST_CHECK(distanceBetweenLineSegments(L1, L2), mm(0.00), "Distance must be 0 mm");
	TEST_CHECK(distanceBetweenLineSegments(L2, L3), mm(1.00), "Distance must be 1 mm");
}

DEFINE_TEST_CASE(PolygonAndLineColl)
{
	Polygon p({ {2.00, 2.00}, { 2.00, 3.00 }, { 3.00, 3.00 }, { 3.00, 2.00 } });
	Polygon p2({ { 2.00, 3.00 }, { 2.00, 7.00 }, { 6.00, 7.00 }, { 4.00, 5.00 }, { 6.00, 3.00 } });
	LineSegment2D l1(FPoint2D(3, 1), FPoint2D(2.50, 2)), l2(FPoint2D(2, 1), FPoint2D(3, 1));
	LineSegment2D l3(FPoint2D(8, 6), FPoint2D(5, 5));
	TEST_CHECK(polygonAndLineSegmentCollides(p, l1), true, "Polygon and line segment must collide");
	TEST_CHECK(polygonAndLineSegmentCollides(p, l2), false, "Polygon and line segment must not collide");
	TEST_CHECK(polygonAndLineSegmentCollides(p2, l3), false, "Polygon and line segment must not collide");
}

DEFINE_TEST_CASE(PolyAndLineAdjacent)
{
	Polygon p({ { 2.00, 2.00 }, { 2.00, 3.00 }, { 3.00, 3.00 }, { 3.00, 2.00 } });
	
	LineSegment2D l1(FPoint2D(3, 1), FPoint2D(2.50, 2)), l2(FPoint2D(2, 1), FPoint2D(3, 1));
	TEST_CHECK(polygonAndLineSegmentAreAdjacent(p, l1, mm(0.50)), true, "Polygon and line segment must adjacent");
	TEST_CHECK(polygonAndLineSegmentAreAdjacent(p, l1, mm(0.10)), true, "Polygon and line segment must adjacent");
	TEST_CHECK(polygonAndLineSegmentAreAdjacent(p, l2, mm(0.50)), false, "Polygon and line segment must not adjacent");
	TEST_CHECK(polygonAndLineSegmentAreAdjacent(p, l2, mm(1.00)), true, "Polygon and line segment must adjacent");
}

DEFINE_TEST_CASE(PolyAndPolyAdjacent)
{
	Polygon p1({ {2.00, 2.00}, { 2.00, 3.00 }, { 3.00, 3.00 }, { 3.00, 2.00 } });
	Polygon p2({ {2.50, 2.50}, { 2.50, 4.00 }, { 4.00, 4.00 }, { 4.00, 2.00 } });
	Polygon p3({ {5.00, 3.00}, { 5.00, 5.00 }, { 7.00, 5.00 }, { 7.00, 4.00 }, { 6.00 , 4.00 }, { 6.00, 3.00 } });

	TEST_CHECK(polygonsAreAdjacent(p1, p2, mm(1.00)), true, "Polygons must adjacent");
	TEST_CHECK(polygonsAreAdjacent(p1, p3, mm(1.00)), false, "Polygons must not adjacent");
	TEST_CHECK(polygonsAreAdjacent(p1, p3, mm(2.00)), true, "Polygons must adjacent");
	TEST_CHECK(polygonsAreAdjacent(p2, p3, mm(1.00)), true, "Polygons must adjacent");
	TEST_CHECK(polygonsAreAdjacent(p2, p3, mm(0.90)), false, "Polygons must not adjacent");
	
}