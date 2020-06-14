#include "../include/Mesh.h"
#include "../include/Polygon.h"
#include "../include/AABB.h"


namespace geom_utils
{

    bool MeshHandler::read(const std::string& filepath, Mesh& mesh)
    {
        auto isAscii = [](std::string filepath) {
        std::string line;
        std::ifstream fromfile(filepath, std::ios::in);
            if (fromfile.is_open())
                while (getline(fromfile, line))
                {
                    if (std::all_of(line.begin(), line.end(), isspace)) continue;
                    if (line.find("solid") != std::string::npos) return true;
                    else return false;
                }
            fromfile.close();
            return false;
        };

        if (isAscii(filepath)) {
            if (!readASCII(filepath, mesh) && !readBinary(filepath, mesh)) 
                return false;
        }
        else
            if (!readBinary(filepath, mesh))
                return false;
        return true;
    }


    bool MeshHandler::readASCII(const std::string& filepath, Mesh& mesh)
    {
        std::string line;
        std::ifstream fromfile;

        fromfile.open(filepath, std::ios::in);
        if (!fromfile.is_open()) {
            fromfile.close();
            return false;
        }

        fromfile >> line;
        fromfile >> line; //comment
        mesh.setModelName(line);

        while (fromfile >> line)
            if (line.find("vertex") != std::string::npos)
            {
                Triangle3D tri;
                fromfile >> tri.a.x >> tri.a.y >> tri.a.z;		   //vertex0 x, y, z
                fromfile >> line >> tri.b.x >> tri.b.y >> tri.b.z; //vertex1 x, y, z
                fromfile >> line >> tri.c.x >> tri.c.y >> tri.c.z; //vertex2 x, y, z
                mesh.add(tri);
            }

        fromfile.close();
        return true;
    }


    bool MeshHandler::readBinary(const std::string& filepath, Mesh& mesh)
    {
        std::ifstream fromfile;
        fromfile.open(filepath, std::ios::in | std::ios::binary);
        if (!fromfile.is_open()) {
            fromfile.close();
            return false;
        }

        fromfile.seekg(0, fromfile.end);
        size_t length = fromfile.tellg();
        fromfile.seekg(0, fromfile.beg);
        size_t actualFacetCount = (length - SIZE_OF_HEADER - sizeof(std::uint32_t)) / SIZE_OF_FACET;

        char header_info[SIZE_OF_HEADER] = "";
        fromfile.read(header_info, SIZE_OF_HEADER);
        mesh.setModelName(header_info);

        unsigned int expectedFacetCount = 0;
        fromfile.read((char*)&expectedFacetCount, 4);

        if (actualFacetCount != expectedFacetCount) return false;
 
        char buffer[SIZE_OF_FACET];
        while (actualFacetCount-- > 0 && fromfile.read(buffer, SIZE_OF_FACET))
        {
            float* ptr = ((float*)buffer);
            FPoint3D normal(*(ptr + 0), *(ptr + 1), *(ptr + 2));
            ptr += 3;
            Triangle3D triangle;
            triangle.a = FPoint3D(*(ptr + 0), *(ptr + 1), *(ptr + 2));
            triangle.b = FPoint3D(*(ptr + 3), *(ptr + 4), *(ptr + 5));
            triangle.c = FPoint3D(*(ptr + 6), *(ptr + 7), *(ptr + 8));
            mesh.add(triangle);
        }

        fromfile.close();
        return true;
    }


    bool MeshHandler::writeASCII(const std::string& filepath, const Mesh& mesh)
    {
        if (mesh.isEmpty()) return false;

        std::ofstream infile;
        infile.open(filepath, std::ios::out | std::ios::trunc);
        if (!infile.is_open()) {
            infile.close();
            return false;
        }

        infile << "solid " << mesh.getModelName();
        for (int i = 0; i < mesh.size(); i++)
        {
            FPoint3D normal = mesh[i].getNormal();
            infile << "\n facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n  outer loop";

            infile << "\n  vertex " << mesh[i].a.x << " " << mesh[i].a.y << " " << mesh[i].a.z;
            infile << "\n  vertex " << mesh[i].b.x << " " << mesh[i].b.y << " " << mesh[i].b.z;
            infile << "\n  vertex " << mesh[i].c.x << " " << mesh[i].c.y << " " << mesh[i].c.z;

            infile << "\n  endloop\n endfacet\n";
        }
        infile << "\nendsolid " << mesh.getModelName();
        infile.close();
        return true;
    }


    bool MeshHandler::writeBinary(const std::string& filepath, const Mesh& mesh)
    {
        if (mesh.isEmpty()) return false;

        std::ofstream infile;
        infile.open(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!infile.is_open()) {
            infile.close();
            return false;
        }

        infile.write(mesh.getModelName().data(), SIZE_OF_HEADER);
        uint32_t facetCount = (uint32_t)mesh.getFacets().size();
        infile.write(reinterpret_cast<const char*>(&facetCount), sizeof(facetCount));

        char buffer[SIZE_OF_FACET] = { 0 };
        for (const auto& tri : mesh.getFacets())
        {
            float* ptr = ((float*)buffer);
            FPoint3D normal = tri.getNormal();

            *(ptr + 0) = normal.x;
            *(ptr + 1) = normal.y;
            *(ptr + 2) = normal.z;
            ptr += 3;
            *(ptr + 0) = tri.a.x; *(ptr + 1) = tri.a.y; *(ptr + 2) = tri.a.z;
            *(ptr + 3) = tri.b.x; *(ptr + 4) = tri.b.y; *(ptr + 5) = tri.b.z;
            *(ptr + 6) = tri.c.x; *(ptr + 7) = tri.c.y; *(ptr + 8) = tri.c.z;
            infile.write(buffer, sizeof(buffer));
        }
        infile.close();
        return true;
    }


    void Mesh::merge(const Mesh& figure)
    {
        std::vector<Triangle3D> facetsFigure(figure.getFacets());
        for (const auto& facet : facetsFigure)
        {
            this->add(facet);
        }
    }


    void Mesh::translate(const FPoint3D& point)
    {
        for (int i = 0; i < facets.size(); i++) 
        {
            facets[i].a += point;
            facets[i].b += point;
            facets[i].c += point;
        }
    }


    AABB3D Mesh::getAABB() const
    {
        AABB3D aabb(*this);
        return aabb;
    }


    Mesh operator+(const Mesh& figure, const Mesh& figure2) 
    {
        Mesh resultFigure;
        resultFigure.merge(figure);
        resultFigure.merge(figure2);
        return resultFigure;
    }


    Mesh operator+=(Mesh& figure, const Mesh& figure2)
    {
        figure.merge(figure2);
        return figure;
    }

}