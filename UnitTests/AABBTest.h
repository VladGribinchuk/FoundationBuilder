#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Point.h"
#include "../GeometryCore/include/AABB.h"
#include "../GeometryCore/include/Polygon.h"

using namespace geom_utils;

DEFINE_TEST_CASE(Calculate) {

        Polygon poly = { FPoint2D(1, 2), FPoint2D(3, 4), FPoint2D(5, 6) };
        AABB2D aabb2d(poly);
        FPoint2D expectedMaxAnswer(5, 6);
        FPoint2D expectedMinAnswer(1, 2);
        TEST_ASSERT((aabb2d.min() == expectedMinAnswer && aabb2d.max() == expectedMaxAnswer), "Failure");
      
       
}
DEFINE_TEST_CASE(isNotValid) {
  
    Polygon poly;
    AABB2D aabb2d(poly);
    TEST_ASSERT(aabb2d.isValid(), "AABB must be not valid!");
}

DEFINE_TEST_CASE(isValid) {
    Polygon poly = { FPoint2D(1, 2), FPoint2D(3, 4), FPoint2D(5, 6) };
    AABB2D aabb2d(poly);
    TEST_ASSERT(aabb2d.isValid(), "AABB must be valid!");

}

DEFINE_TEST_CASE(countainsFpoint2DInAABB) {

    AABB2D aabb2d(FPoint2D(1, 2), FPoint2D(7, 7));
    FPoint2D point2d(2, 4);
    TEST_ASSERT(aabb2d.contains(point2d), "Failure");
}

DEFINE_TEST_CASE(countainsFpoint3DInAABB) {

    AABB3D aabb3d(FPoint3D(1, 1, 1), FPoint3D(4, 4, 4));
    FPoint3D point3d(2, 2, 3);
    TEST_ASSERT(aabb3d.contains(point3d), "Failure");
}