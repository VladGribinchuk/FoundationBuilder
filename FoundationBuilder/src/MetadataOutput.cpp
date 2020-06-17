#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../include/MetadataOutput.h"
#include "../../GeometryCore/include/Triangle.h"
#include "../../GeometryCore/include/Polygon.h"
#include "../../GeometryCore/include/Mesh.h"
#include "../../GeometryCore/include/AABB.h"

namespace geom_utils {
    void writeMetadata(const Mesh& inputModel, const Mesh& foundation, const std::string& metadataFile, const std::string& inputModelName, const std::string& foundationName, const bool& buildIn, const float& timeOfConstructing, const float& foundationMinimalZCoord) {
        std::ofstream metaOutput(metadataFile);

        std::stringstream inputModelBoundBox;
        std::stringstream foundationBoundBox;
        auto inputModelAABB = inputModel.getAABB();
        auto foundationAABB = foundation.getAABB();
        float height = foundationAABB.pmax.z - foundationAABB.pmin.z;
        inputModelBoundBox << "[" << inputModelAABB.pmin.x << ", " << inputModelAABB.pmax.x << ", " << inputModelAABB.pmin.y << ", " << inputModelAABB.pmax.y << ", " << inputModelAABB.pmin.z << ", " << inputModelAABB.pmax.z << "]";
        foundationBoundBox << "[" << foundationAABB.pmin.x << ", " << foundationAABB.pmax.x << ", " << foundationAABB.pmin.y << ", " << foundationAABB.pmax.y << ", " << foundationMinimalZCoord << ", " << foundationMinimalZCoord+height << "]";

        auto foundationFacets = foundation.getFacets();
        Polygon foundationTopPolygon;
        for (int i = 0; i < foundationFacets.size() - 3; i += 4) {
            if (i == 0) {
                foundationTopPolygon.add(FPoint2D(foundationFacets[i].b.x, foundationFacets[i].b.y));
            }
            foundationTopPolygon.add(FPoint2D(foundationFacets[i].c.x, foundationFacets[i].c.y));
            if (i == foundationFacets.size() - 4) {
                foundationTopPolygon.add(FPoint2D(foundationFacets[i].a.x, foundationFacets[i].a.y));
            }
        }
        if (foundationTopPolygon.orientation()) {
            std::reverse(foundationTopPolygon.begin(), foundationTopPolygon.end());
        }
        float polygonLength = foundationTopPolygon.polygonLength();

        float polygonArea = foundationTopPolygon.area();
        int numOfFacets = foundation.getFacets().size();

        metaOutput << "{" << std::endl \
            << "\t\"model\": {" << std::endl \
            << "\t\tname: \"" << inputModelName << "\"," << std::endl \
            << "\t\tbbox: " << inputModelBoundBox.str() << std::endl \
            << "\t}," << std::endl \
            << "\t\"foundation\": {" << std::endl\
            << "\t\tname: \""  << foundationName << "\"," << std::endl \
            << "\t\tbbox: " << foundationBoundBox.str() << "," << std::endl \
            << "\t\tis_build_in: " << (buildIn ? "true" : "false") << "," << std::endl \
            << "\t\theight: " << height << "," << std::endl \
            << "\t\tpolygon_length: " << polygonLength << "," << std::endl \
            << "\t\tpolygon_area: " << polygonArea << "," << std::endl \
            << "\t\tnum_of_facets: " << numOfFacets << "," << std::endl \
            << "\t\ttime_of_constructing: " << timeOfConstructing << std::endl \
            << "\t}" << std::endl \
            << "}" << std::endl;
        metaOutput.close();
    }
}