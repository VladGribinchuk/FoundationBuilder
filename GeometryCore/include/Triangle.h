#pragma once

#include "Point.h"

namespace geom_utils
{
    struct Triangle2D
    {
        Triangle2D(const FPoint2D& a, const FPoint2D& b, const FPoint2D& c)
            : a(a), b(b), c(c)
        {}

        FPoint2D a, b, c;
    };

    struct Triangle3D
    {
        Triangle3D(const FPoint3D& a, const FPoint3D& b, const FPoint3D& c)
            : a(a), b(b), c(c)
        {}
        Triangle3D() = default;

        FPoint3D a, b, c;
    };
}