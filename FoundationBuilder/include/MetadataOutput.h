#pragma once
#include <string>

namespace geom_utils {
	class Mesh;
	void writeMetadata(const Mesh& inputModel, const Mesh& foundation, const std::string& metadataFile, const std::string& inputModelName, const std::string& foundationName, const bool& buildIn, const float& timeOfConstructing, const float& foundationMinimalZCoord);
}