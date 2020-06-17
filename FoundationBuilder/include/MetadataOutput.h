#pragma once
#include <string>

namespace geom_utils {
	class Mesh;
	void writeMetadata(const Mesh& inputModel, const Mesh& foundation, std::string metadataFile, std::string inputModelName, std::string foundationName, bool buildIn, float timeOfConstructing, float foundationMinimalZCoord);
}