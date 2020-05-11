#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Polygon.h"

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