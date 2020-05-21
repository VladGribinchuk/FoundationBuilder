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

        void setModelName(const std::string& str) { this->modelName = str; }
        std::string getModelName() const { return this->modelName; }

        const Triangle3D& operator[] (unsigned index) const { return facets[index]; }
        Triangle3D& operator[] (unsigned index) { return facets[index]; }

    };

    class MeshHandler
    {
        static Mesh readBinary(const std::string& filepath);
        static Mesh readASCII(const std::string& filepath);
    public:
        static Mesh read(const std::string& filepath);
        static void write(const std::string& filepath, const Mesh& mesh);
    };
}