#pragma once
#include <fstream>
#include "Triangle.h"
#include "Point.h"
#include <vector>
#include <string>

namespace geom_utils
{
    class Mesh
    {
        std::vector<Triangle3D> facets;
        std::string modelName;
        
    public:
        Mesh() = default; //empty mesh
        bool isEmpty() const { return facets.empty(); }
        void add(const Triangle3D& p) { facets.push_back(p); }
        size_t size() const { return this->facets.size(); }
        void clear() { facets.clear(); }
        const std::vector<Triangle3D>& getFacets() const { return facets; }

        void setModelName(const std::string& str) { this->modelName = str; }
        std::string getModelName() const { return this->modelName; }

        const Triangle3D& operator[] (unsigned index) const { return facets[index]; }
        Triangle3D& operator[] (unsigned index) { return facets[index]; }

        bool read(const std::string& filepath);
        bool writeASCII(const std::string& filepath) const;
        bool writeBinary(const std::string& filepath) const;
    };

    Mesh createFoundation(const Mesh& inputModel, const FPoint3D::coord foundationHeight, const FPoint3D::coord inflateValue);

    class MeshHandler
    {
        // used for binary STL
        static constexpr size_t SIZE_OF_HEADER = 80; // bytes
        static constexpr size_t SIZE_OF_FACET = 50; // 12 bytes for normal + 3 * (12 bytes per vertex) + 2 bytes for attribute

        static bool readBinary(const std::string& filepath, Mesh& mesh);
        static bool readASCII(const std::string& filepath, Mesh& mesh);
    public:
        static bool read(const std::string& filepath, Mesh &mesh);      
        static bool writeASCII(const std::string& filepath, const Mesh& mesh);
        static bool writeBinary(const std::string& filepath, const Mesh& mesh);
    };


    inline bool Mesh::read(const std::string& filepath)
    {
        clear();
        return MeshHandler::read(filepath, *this);
    }

    inline bool Mesh::writeASCII(const std::string& filepath) const
    {
        return MeshHandler::writeASCII(filepath, *this);
    }

    inline bool Mesh::writeBinary(const std::string& filepath) const
    {
        return MeshHandler::writeBinary(filepath, *this);
    }
}