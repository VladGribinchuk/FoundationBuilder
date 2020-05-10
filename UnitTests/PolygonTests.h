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
