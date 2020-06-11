#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Triangle.h"

using namespace geom_utils;


DEFINE_TEST_CASE(TriangleGetNormal3D)
{

   Triangle3D tri( {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} );
   FPoint3D f = tri.getNormal();
   TEST_ASSERT(f.x == 0 && f.y == -1 && f.z == 0, "The normal must be 0, 0, 1!");
}

DEFINE_TEST_CASE(TriangleReverse)
{
    Triangle3D test_triangle({ 1, 1, 0 }, { 0, 1, 0 }, { 0, 1, -1 });
    test_triangle.reverse();
    TEST_ASSERT(test_triangle.getNormal() == FPoint3D(0, 1, 0), "Normal of reversed triangle must be 0, 1, 0");

}