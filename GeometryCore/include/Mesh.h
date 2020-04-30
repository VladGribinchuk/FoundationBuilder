#pragma once

#include "Point.h"
#include <vector>
#include <string>

namespace geom_utils
{
    class Mesh
    {
        std::vector<FPoint3D> vertices;
    public:
        Mesh() = default; //empty mesh

        bool isEmpty() const { return vertices.empty(); }
    };

    class MeshHandler
    {
    public:
        static Mesh read(const std::string& filepath) { return {}; }
        static void write(const Mesh& filepath) { }
    };
}