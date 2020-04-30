#pragma once

namespace geom_utils
{
    enum class GeometryExecEnvironment
    {
        SINGLE_THREADED,
        MULTI_THREADED_OPENMP,
        MULTI_THREADED_INTEL_CILK
    };

    // SINGLE_THREADED by default.
    // Returns old environment value.
    GeometryExecEnvironment setGeometryExecEnvironment(GeometryExecEnvironment newEnv);

    GeometryExecEnvironment getGeometryExecEnvironment();

} //namespace geom_utils
