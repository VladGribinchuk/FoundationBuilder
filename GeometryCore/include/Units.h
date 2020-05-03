#pragma once

namespace geom_utils
{
    using coord_t = long long int;
    using um = coord_t; // microns
    using mm = float;   // millimeters

    // By default is 0.001mm, i.e. 1um
    float getUnitTolerance();
    void setUnitTolerance(float newTolerance);
}
