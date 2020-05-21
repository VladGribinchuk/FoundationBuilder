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
