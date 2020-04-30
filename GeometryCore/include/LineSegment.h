#pragma once

#include "Point.h"

namespace geom_utils
{
    struct LineSegment2D
    {
        LineSegment2D(const FPoint2D& a, const FPoint2D& b)
            : a(a), b(b)
        {}

        FPoint2D a, b;
        typedef typename FPoint2D::coord coord;
    };
}