#include "UnitTests.h"
#include "../GeometryCore/include/Point.h"
#include "../GeometryCore/include/LinearAlgebra.h"
#include "../GeometryCore/include/SVGWriter.h"
#include "../GeometryCore/include/LineSegment.h"

DEFINE_TEST_CASE(PointSum)
{
    geom_utils::FPoint2D p1(1, 2);
    geom_utils::FPoint2D p2(1, 2);
    ASSERT(p1 + p2 == geom_utils::FPoint2D(2, 4), "Points sum must be FPoint2D(2, 4)!");
}

DEFINE_TEST_CASE(PointDistance)
{
    geom_utils::FPoint2D p1(100, 200);
    geom_utils::FPoint2D p2(200, 200);
    CHECK(geom_utils::distance(p1, p2), geom_utils::mm(100.00), "Distance must be 100 mm!");
}

int main()
{
    RUN_ALL_TESTS();

    return 0;
}