#pragma once

namespace geom_utils
{
    enum class [[deprecated("GeometryExecEnvironment is depricated and should not be used.")]] GeometryExecEnvironment
    {
        SINGLE_THREADED,
        MULTI_THREADED_OPENMP,
        MULTI_THREADED_INTEL_CILK
    };

    // SINGLE_THREADED by default.
    // Returns old environment value.
    [[deprecated("setGeometryExecEnvironment is depricated and should not be used.")]] GeometryExecEnvironment setGeometryExecEnvironment(GeometryExecEnvironment newEnv) {};

    [[deprecated("getGeometryExecEnvironment is depricated and should not be used.")]] GeometryExecEnvironment getGeometryExecEnvironment() { return {}; };

} //namespace geom_utils
