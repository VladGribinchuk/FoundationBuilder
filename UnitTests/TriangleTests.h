#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Triangle.h"

using namespace geom_utils;


DEFINE_TEST_CASE(TriangleGetNormal3D)
{

   Triangle3D tri( {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} );
   FPoint3D f = geom_utils::getNormal(tri);
   TEST_ASSERT(f.x == 0 && f.y == 0 && f.z == 1, "The normal must be 0, 0, 1!");
}

DEFINE_TEST_CASE(TriangleGetNormal2D)
{
    Triangle2D tri({ 1, 1 }, { 0, 1 }, { 0, -1 });
    FPoint2D f = geom_utils::getNormal(tri);
    TEST_ASSERT(f.x == 0 && f.y == 1, "The normal must be 0, 1!");
}