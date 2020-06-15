#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Polygon.h"
#include <ctime>

using namespace geom_utils;

class RandomPolyGen
{
    Polygon poly;
public:
    RandomPolyGen(size_t size)
    {
        srand(time(NULL));
        while (size-->0)
            poly.add({ mm(rand() % 100), mm(rand() % 100) });
    }
    const Polygon& getPoly() const { return poly; }
};

// randomPoly1000 has internal linkage
// Keep it declared on top of all other stuff to not pollute test results with time needed for generation of polygon.
// It will be initialized before any test cases below and can be accessed from test cases.
namespace {
#ifdef NDEBUG // larger stress loading for the Release build
    RandomPolyGen randomPoly(10000);
    RandomPolyGen monsterPoly(100000);
#else
    RandomPolyGen randomPoly(1000);
#endif
}

constexpr size_t ITERATIONS_COUNT_10 = 10;

DEFINE_TEST_GROUP_BEGIN(Performance_tests)

#ifdef NDEBUG // only for Release
DEFINE_TEST_CASE_TIME_STAMP(ConvexHull_MonsterPoly)
{
    monsterPoly.getPoly().convexHull();
}
#endif

DEFINE_TEST_CASE_TIME_STAMP_REAPETABLE(ConvexHull_RandomPoly1000, ITERATIONS_COUNT_10)
{
    randomPoly.getPoly().convexHull();
}

// You can check that area computation is realy fast.
// CPU works very quickly when it has to deal only with sum and multiplication of numbers.
DEFINE_TEST_CASE_TIME_STAMP(PolygonArea_RandomPoly1000)
{
    randomPoly.getPoly().area();
}

DEFINE_TEST_GROUP_END()
