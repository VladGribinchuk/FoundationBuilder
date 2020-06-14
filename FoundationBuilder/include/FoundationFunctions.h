#pragma once
#include "../../GeometryCore/include/Mesh.h"
#include "../../GeometryCore/include/Point.h"
#include "../../GeometryCore/include/Units.h"

#include "../../GeometryCore/include/Polygon.h"
#include "../../GeometryCore/include/AABB.h"
#include "../../GeometryCore/include/Triangle.h"

namespace geom_utils
{
    /*
    * NOTE:The method creates a model of foundation for input figure
    * The height of foundation = "foundationHeight";
    * The foundation is wider than input model by "inflateValue" value;
    */
    Mesh createFoundation(const Mesh& inputModel, const FPoint3D::coord foundationHeight, const FPoint3D::coord inflateValue);

    /*
     * NOTE:Method works correctly only if there are two types z(z0,z1)
     *
     * The foundation is placed under the figure with a gap
     *  - by default gap is equal to 0.5
     */
    Mesh integrateFoundationIntoModel(const Mesh& model, Mesh foundation);
}
