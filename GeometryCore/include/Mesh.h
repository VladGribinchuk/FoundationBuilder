#pragma once

#include "Point.h"
#include "Triangle.h"
#include <vector>
#include <string>
#include <functional>

namespace geom_utils
{
    class Mesh
    {
        std::vector<Triangle3D> triangles;
    public:
        Mesh() = default; //empty mesh

        void add(const Triangle3D& triangle) { triangles.push_back(triangle); }
        void clear() { triangles.clear(); }
        const std::vector<Triangle3D>& getTriangles() const { return triangles; }
        bool isEmpty() const { return triangles.empty(); }

        bool read(const std::string& from);
        bool write(const std::string& to) const;
    };

    /*
     * MeshHandler uses callbacks system to notify about events during reading of STL file.
     * It may be handy for smooth interaction with some logging system.
     */
    class MeshHandler
    {
        // used for binary STL
        static constexpr size_t SIZE_OF_HEADER = 80; // bytes
        static constexpr size_t SIZE_OF_TRIS_NUM = sizeof(std::uint32_t);
        static constexpr size_t SIZE_OF_FACET = 50; // 12 bytes for normal + 3 * (12 bytes per vertex) + 2 bytes for attribute

    public:
        using BeginSolidCallback_t = std::function<void(std::string)>;
        using EndSolidCallback_t = std::function<void()>;
        using TriangleParsedCallback_t = std::function<void(Triangle3D)>;
        using TraceCallback_t = std::function<void(std::string)>;

        /*
         * Callbacks:
         * - BeginSolidCallback_t - called when starts read STL model, pass solid name
         * - EndSolidCallback_t - called at the end of reading
         * - TriangleParsedCallback_t - called when new triangle is parsed, pass Triangle3D
         * - TraceCallback_t - called when warning or error occured, pass string containing message
         */
        explicit MeshHandler(TriangleParsedCallback_t triangleParsedCallback = [](Triangle3D) {},
            TraceCallback_t traceCallback = [](std::string) {},
            BeginSolidCallback_t beginSolidCallback = [](std::string){},
            EndSolidCallback_t endSolidCallback = [](){})
            : triangleParsed(triangleParsedCallback), trace(traceCallback), beginSolid(beginSolidCallback), endSolid(endSolidCallback)
        { }

        /*
         * MeshHandler is able to read binary STL format as well as ascii format,
         * but parsing algorithm is quite simple for now, thus, will work with some restrictions.
         * 
         * RESTRICTIONS: ascii STL should contain each record starting with new line, in other case it won't parse.
         * Below is example:
         * 
         * solid name \n
         *   facet normal ni nj nk \n
         *   outer loop \n
         *     vertex v1x v1y v1z \n
         *     vertex v2x v2y v2z \n
         *     vertex v3x v3y v3z \n
         *   endloop \n
         *   endfacet \n
         * endsolid name
         */
        bool read(const std::string& filepath) const;

        /*
         * Write STL only in binary format
         */
        bool write(const Mesh& mesh, const std::string& filepath) const;

    private:
        bool readBinary(const std::string& filepath) const;
        bool readAscii(const std::string& filepath) const;

    private:
        BeginSolidCallback_t beginSolid;
        EndSolidCallback_t endSolid;
        TriangleParsedCallback_t triangleParsed;
        TraceCallback_t trace;
    };

    inline bool Mesh::read(const std::string& from)
    {
        clear();
        return MeshHandler{ std::bind(&Mesh::add, this, std::placeholders::_1) }.read(from); // register only callback for triangle parsing, skip the rest
    }

    inline bool Mesh::write(const std::string& to) const
    {
        return MeshHandler{ }.write(*this, to); // skip all callbacks
    }
}