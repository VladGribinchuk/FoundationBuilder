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
        std::vector<Triangle3D> vertices;
        std::string comment;
        
    public:
        Mesh() = default; //empty mesh
        bool isEmpty() const { return vertices.empty(); }
        void add(const Triangle3D& p) { vertices.push_back(p); }
        size_t size() const { return this->vertices.size(); }

        void setComment(const std::string& str) { this->comment = str; }
        std::string getComment() const { return this->comment; }

        const Triangle3D& operator[] (unsigned index) const { return vertices[index]; }
        Triangle3D& operator[] (unsigned index) { return vertices[index]; }

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