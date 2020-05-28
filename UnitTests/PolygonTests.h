#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Polygon.h"
#include "../GeometryCore/include/Triangle.h"

using namespace geom_utils;

DEFINE_TEST_CASE(PolygonOutput)
{
    Polygon poly = { FPoint2D(1, 2), FPoint2D(3, 4), FPoint2D(5, 6) };
    TEST_OUTPUT() << poly;
    TEST_CHECK((std::stringstream{} << poly).str(), std::string("{ {1.000; 2.000}; {3.000; 4.000}; {5.000; 6.000} }"), "Polygon output operation is incorrect!");
    TEST_CHECK((std::stringstream{} << Polygon{}).str(), std::string("{ }"), "Polygon output operation for empty polygon is incorrect!");
}

DEFINE_TEST_CASE(PolygonInput)
{
    Polygon poly;
    std::stringstream ss;
    ss.str("{{1.000; 2.000}; {3.000; 4.000}; {5.000; 6.000}}");
    ss >> poly;
    Polygon expected = { FPoint2D(1, 2), FPoint2D(3, 4), FPoint2D(5, 6) };
    // assert below won't work for now, because operator== isn't completed yet
    // TEST_ASSERT(poly == expected, "Polygon input operation is incorrect!");

    ss.str("{}");
    ss >> poly;
    expected = Polygon{};
    // assert below won't work for now, because operator== isn't completed yet
    // TEST_ASSERT(poly == expected, "Polygon input operation for empty polygon is incorrect!");
}

DEFINE_TEST_CASE(PolygonInput_CorruptedData)
{
    Polygon poly;
    std::stringstream ss;
    ss.str("{{1.000; 2.000}; {3.000; 4.000}  {5.000; 6.000}}");
    //                                     ^ missing semicolon
    // expected output should be valid polygon containing only first two points
    ss >> poly;
    Polygon expected = { FPoint2D(1, 2), FPoint2D(3, 4) };
    // assert below won't work for now, because operator== isn't completed yet
    // TEST_ASSERT(poly == expected, "Polygon input operation is incorrect!");
}

DEFINE_TEST_CASE(PolygonAddRange)
{
    std::stringstream ss;
    Polygon p;
    p.addRange({ {2.000, 5.000 } , {4.000, 8.000 } , {3.000, 11.000 } });
    std::for_each(p.begin(), p.end(), [&](auto el) {ss << el; });
    TEST_CHECK((ss).str(), std::string("{2.000; 5.000}{4.000; 8.000}{3.000; 11.000}"), "The polygon points must be {2.000; 5.000}{4.000; 8.000}{3.000; 11.000}!");
}

DEFINE_TEST_CASE(PolygonOrientation)
{
    Polygon p({ {-2.000, 5.000}, { -1.000, -2.000 }, { 3.000, -1.000 }, { 3.000, 5.000 } });
    TEST_ASSERT(p.orientation(), "Orientation must return <true>!");
}

DEFINE_TEST_CASE(PolygonOrientation_false)
{
    Polygon p({ {-2.000, 5.000}, { 3.000, 5.000 }, { 3.000, -1.000 }, { -1.000, -2.000 } });
    TEST_ASSERT(!(p.orientation()), "Orientation must be clockwise!");
}

DEFINE_TEST_CASE(PolygonConvexHull)
{
    std::stringstream ss;
    Polygon p({ {-2.000, 5.000}, { 1.000, 3.000 }, { 1.000, 1.000 }, { 2.000, 2.000 }, { 3.000, -1.000 }, { -1.000, -2.000 }, { 3.000, 5.000 } });
    Polygon convex_p = p.convexHull();
    std::for_each(convex_p.begin(), convex_p.end(), [&](auto el) {ss << el; });
    TEST_CHECK((ss).str(), std::string("{-2.000; 5.000}{-1.000; -2.000}{3.000; -1.000}{3.000; 5.000}"), "The convex hull must be {-2.000; 5.000}{-1.000; -2.000}{3.000; -1.000}{3.000; 5.000}!");
}

DEFINE_TEST_CASE(PolygonIsConvexHull)
{
    Polygon p({ {2, 5}, {-3, 3}, {-4,-2}, {-4,-3}, {-2,-3}, {3,0} });
    TEST_ASSERT(p.isConvexHull(), "The polygon must be convex!");
}

DEFINE_TEST_CASE(PolygonIsConvexHull_false)
{
    Polygon p({ {2, 5}, {-3, 3}, {-4,-2}, {0,0}, {-2,-3}, {3,0} });
    TEST_ASSERT(!(p.isConvexHull()), "The polygon must be non-convex!");
}

DEFINE_TEST_CASE(PolygonArea)
{
    Polygon p({ {2, 5}, {-3, 3}, {-4,-2}, {-4,-3}, {-2,-3}, {3,0} });
    TEST_ASSERT(p.area() == -36.5, "The area must be -36.500!");
}

DEFINE_TEST_CASE(PolygonEqual)
{
    Polygon p({ { 0, 0 }, { 0, 4 }, { 4, 4 }, { 4, 0 } });
    Polygon p2({ { 0, 4 }, { 4, 4 }, { 4, 0 }, { 0, 0 } });

    TEST_ASSERT(p == p2, "Polygons p and p2 must be equal");
}

DEFINE_TEST_CASE(PolygonNotEqual)
{
    Polygon p1({ { 0, 4 }, { 4, 4 }, { 4, 0 }, { 0, 0 } });
    Polygon p2({ { 4, 4 }, { 4, 0 }, { 0, 0 } });
    Polygon p3({ { 4, 4 }, { 0, 4 }, { 4, 0 }, { 0, 0 } });
    
    TEST_ASSERT(p1 != p2, "Polygons p1 and p2 must not be equal");
    TEST_ASSERT(p1 != p3, "Polygons p1 and p3 must not be equal");
}

DEFINE_TEST_CASE(PolygonLength)
{
    Polygon p1({});
    Polygon p2({ {1, 1} });
    Polygon p3({ {1, 1}, {1, 3} });
    Polygon p4({ {1, 1}, {1, 3}, {6, 3}, {6, 1} });

    TEST_ASSERT(p1.polygonLength() == 0, "Polygon p1 length must be 0");
    TEST_ASSERT(p2.polygonLength() == 0, "Polygon p2 length must be 0");
    TEST_ASSERT(p3.polygonLength() == 2, "Polygon p3 length must be 2");
    TEST_ASSERT(p4.polygonLength() == 14, "Polygon p4 length must be 14");
    
}

DEFINE_TEST_CASE(PolygonCentroid) {
    Polygon p1({ { 0, 4 }, { 4, 4 }, { 4, 0 }, { 0, 0 } });
    Polygon p2({ {1, 0}, {2, 0}, {0, 3} });
    Polygon p3({ {1, 0}, {2, 1}, {0, 3}, {-1, 2}, {-2, -1} });

    TEST_ASSERT(p1.centroid() == FPoint2D(mm(2.00), mm(2.00)), "Polygon p1 centroid must be in point {2.00, 2.00}");
    TEST_ASSERT(p2.centroid() == FPoint2D(mm(1.00), mm(1.00)), "Polygon p2 centroid must be in point {1.00, 1.00}");
    TEST_ASSERT(p3.centroid() == FPoint2D(mm(-0.083), mm(0.917)), "Polygon p3 centroid must be in point {-0.083, 0.917}");
}

DEFINE_TEST_CASE(PolygonClosestTo)
{
    Polygon p1({ {0, 0}, {4, 2}, {6, 0} });
    
    FPoint2D point1(4.50, 0);
    
    TEST_ASSERT(p1.closestTo(point1) == FPoint2D(mm(6.00), mm(0.00)), "Closest point of p1 to point1 must be {6.00, 0.00}");
}

DEFINE_TEST_CASE(PolygonTranslate)
{
    Polygon p1({ {0, 0}, {3, 4}, {9, -3} });
    Polygon p2({ {-1, 2}, {2, 6}, {8, -1} });
    Polygon p3({});
    Polygon p4({});
    p1.translate(FPoint2D(mm(-1.00), mm(2.00)));
    TEST_ASSERT(p1 == p2, "Polygon p1 after translation must be in place where is p2");
    p3.translate(FPoint2D(mm(-1.00), mm(2.00)));
    TEST_ASSERT(p3 == p4, "Empty polygon p3 after translation must be empty polygon");
}


DEFINE_TEST_CASE(PolygonTriangulation)
{
    Polygon poly ({ {-3,-4},{-4, 0}, {0, 4}, {4,0}, {3,-4} });


    // {-4,0}, {-3,-4},{0, 4}, {4, 0}, {3,-4}

    /*std::vector <Triangle2D> vectorForTriangulate = { 
        Triangle2D(FPoint2D(-3,-4),FPoint2D(0,0),FPoint2D(-4,0)), 
        Triangle2D(FPoint2D(-4,0),FPoint2D(0,0),FPoint2D(0,4)), 
        Triangle2D(FPoint2D(0,4),FPoint2D(0,0),FPoint2D(4,0)),
        Triangle2D(FPoint2D(4,0),FPoint2D(0,0),FPoint2D(3,-4)),
        Triangle2D(FPoint2D(3,-4),FPoint2D(0,0),FPoint2D(-3,-4))
    };*/
    std::vector <Triangle2D> vectorForTriangulate = {
        Triangle2D(FPoint2D(-4,0),FPoint2D(-3,-4),FPoint2D(0,4)),
        Triangle2D(FPoint2D(0,4),FPoint2D(-3,-4),FPoint2D(4,0)),
        Triangle2D(FPoint2D(4,0),FPoint2D(-3,-4),FPoint2D(3,-4))
    };


    std::vector<Triangle2D> tris = poly.triangulate();


   /* TEST_ASSERT(
        tris[0].a == vectorForTriangulate[0].a && tris[0].b == vectorForTriangulate[0].b && tris[0].c == vectorForTriangulate[0].c && 
        tris[1].a == vectorForTriangulate[1].a && tris[1].b == vectorForTriangulate[1].b && tris[1].c == vectorForTriangulate[1].c &&
        tris[2].a == vectorForTriangulate[2].a && tris[2].b == vectorForTriangulate[2].b && tris[2].c == vectorForTriangulate[2].c &&
        tris[3].a == vectorForTriangulate[3].a && tris[3].b == vectorForTriangulate[3].b && tris[3].c == vectorForTriangulate[3].c &&
        tris[4].a == vectorForTriangulate[4].a && tris[4].b == vectorForTriangulate[4].b && tris[4].c == vectorForTriangulate[4].c &&
        tris[5].a == vectorForTriangulate[5].a && tris[5].b == vectorForTriangulate[5].b && tris[5].c == vectorForTriangulate[5].c, 
        "Failed");*/
    TEST_ASSERT(
        tris[0].a == vectorForTriangulate[0].a && tris[0].b == vectorForTriangulate[0].b && tris[0].c == vectorForTriangulate[0].c &&
        tris[1].a == vectorForTriangulate[1].a && tris[1].b == vectorForTriangulate[1].b && tris[1].c == vectorForTriangulate[1].c &&
        tris[2].a == vectorForTriangulate[2].a && tris[2].b == vectorForTriangulate[2].b && tris[2].c == vectorForTriangulate[2].c ,
        "Failed");
}
