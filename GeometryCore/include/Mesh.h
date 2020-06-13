#pragma once
#include <fstream>
#include "Triangle.h"
#include "Point.h"
#include <vector>
#include <string>


namespace geom_utils
{
    class AABB3D;
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

       
        // Translate mesh to the given point
        void translate(const FPoint3D& point);
        //add facets to mesh object
        void merge(const Mesh& figure);

        AABB3D getAABB() const;

        
        
    };

    Mesh operator+(const Mesh& figure, const Mesh& figure2);
    Mesh operator+=(Mesh& figure, const Mesh& figure2);
    


    Mesh createFoundation(const Mesh& inputModel, const FPoint3D::coord foundationHeight, const FPoint3D::coord inflateValue);
    /*
     * NOTE:Method works correctly only if there are two types z(z0,z1)
     *
     * The foundation is placed under the figure with a gap
     *  - by default gap is equal to 0.5
     */
    Mesh integrateFoundationIntoModel(const Mesh& model, Mesh& foundation);

    class MeshHandler
    {
        // used for binary STL
        static constexpr size_t SIZE_OF_HEADER = 80; // bytes
        static constexpr size_t SIZE_OF_FACET = 50; // 12 bytes for normal + 3 * (12 bytes per vertex) + 2 bytes for attribute

        static bool readBinary(const std::string& filepath, Mesh& mesh);
        static bool readASCII(const std::string& filepath, Mesh& mesh);
    public:
        static bool read(const std::string& filepath, Mesh& mesh);
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