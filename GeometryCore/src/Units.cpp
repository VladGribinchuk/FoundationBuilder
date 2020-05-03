#include "../include/Units.h"

namespace {
    float tolerance = 0.001;
}

namespace geom_utils
{
    float getUnitTolerance()
    {
        return tolerance;
    }

    void setUnitTolerance(float newTolerance)
    {
        tolerance = newTolerance;
    }
}