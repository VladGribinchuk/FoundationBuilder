#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Point.h"
#include "../GeometryCore/include/LinearAlgebra.h"

using namespace geom_utils;

DEFINE_TEST_CASE(PointSum)
{
    FPoint2D p1(1, 2);
    FPoint2D p2(1, 2);
    TEST_ASSERT(p1 + p2 == FPoint2D(2, 4), "Points sum must be FPoint2D(2, 4)!");
    p1 += p2;
    TEST_ASSERT(p1 == FPoint2D(2, 4), "Points sum and assign must be FPoint2D(2, 4)!");
}

DEFINE_TEST_CASE(PointSubstraction)
{
    FPoint2D p1(3, 4);
    FPoint2D p2(1, 2);
    TEST_ASSERT(p1 - p2 == FPoint2D(2, 2), "Points substruction must be FPoint2D(2, 2)!");
    p1 -= p2;
    TEST_ASSERT(p1 == FPoint2D(2, 2), "Points substruction and assign must be FPoint2D(2, 2)!");
}

DEFINE_TEST_CASE(PointNegate)
{
    FPoint2D p1(1, 2);
    FPoint2D p2 = -p1;
    TEST_ASSERT(p2 == FPoint2D(-1, -2), "Points negate must be FPoint2D(-1, -2)!");
}

DEFINE_TEST_CASE(PointMult)
{
    FPoint2D p(1, 2);
    TEST_ASSERT(p * 2 == FPoint2D(2, 4), "Points mult must be FPoint2D(2, 4)!");
}

DEFINE_TEST_CASE(PointComparison)
{
    FPoint2D p1(1, 2);
    FPoint2D p2(1, 2);
    FPoint2D p3(3, 4);
    TEST_ASSERT(p1 == p2, "Points p1 and p2 should be equal!");
    TEST_ASSERT(p2 != p3, "Points p2 and p3 shouldn't be equal!");
}

DEFINE_TEST_CASE(PointDistance)
{
    FPoint2D p1(100, 200);
    FPoint2D p2(200, 200);
    TEST_CHECK(distance(p1, p2), mm(100.00), "Distance must be 100 mm!");
}

DEFINE_TEST_CASE(PointOutput)
{
    FPoint2D p(1, 2);
    TEST_OUTPUT() << p;
    TEST_CHECK((std::stringstream{} << p).str(), std::string("{1.000; 2.000}"), "Output for FPoint2D p(1, 2) should be {1.000, 2.000}!");
}

DEFINE_TEST_CASE(PointInput)
{
    std::stringstream ss;
    ss.str("{1.000; 2.000}");
    FPoint2D p = notAPoint<FPoint2D>();
    ss >> p;
    TEST_ASSERT(p == FPoint2D(1, 2), "Point p should be {1.000, 2.000}!");
}
