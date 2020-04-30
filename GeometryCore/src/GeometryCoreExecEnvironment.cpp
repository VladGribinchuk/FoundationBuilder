#include "../include/GeometryCoreExecEnvironment.h"

namespace { // anonymous namespace to make environment variable visible only this translation unit.
    geom_utils::GeometryExecEnvironment environment = geom_utils::GeometryExecEnvironment::SINGLE_THREADED;
}

namespace geom_utils
{
    GeometryExecEnvironment setGeometryExecEnvironment(GeometryExecEnvironment newEnv)
    {
        auto oldEnv = environment;
        environment = newEnv;
        return oldEnv;
    }

    GeometryExecEnvironment getGeometryExecEnvironment()
    {
        return environment;
    }

} //namespace geom_utils