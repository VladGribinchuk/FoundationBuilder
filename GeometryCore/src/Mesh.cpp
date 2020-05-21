#include "../include/Mesh.h"
#include <fstream>
#include <sstream>

namespace geom_utils
{
    bool MeshHandler::read(const std::string& filepath) const
    {
        auto isAsciiSolid = [](std::string filepath) {
            std::ifstream f(filepath);
            // skip any whitespaces at the beggining of the file
            char c;
            size_t spaces = 0;
            while (f.get(c) && std::isspace(c))
                ++spaces;
            f.seekg(spaces);
            char header[6] = { '\0' };
            f.read(header, 5);
            // if file starts with 'solid', then, most likely, it is in ascii format
            return std::string(header) == std::string("solid");
        };

        if (isAsciiSolid(filepath))
        {
            // file can start with 'solid', but still be in binary format, so additionaly try read_binary in this case
            if (!readAscii(filepath) && !readBinary(filepath))
            {
                trace("Cannot parse file " + filepath);
                return false;
            }
        }
        else if (!readBinary(filepath))
        {
            trace("Cannot parse file " + filepath);
            return false;
        }
        return true;
    }

    bool MeshHandler::write(const Mesh& mesh, const std::string& filepath) const
    {
        std::ofstream f(filepath, std::ios::out | std::ios::binary);
        if (!f.is_open())
            return false;

        char header[SIZE_OF_HEADER] = { 0 };
        f.write(header, sizeof(header));

        uint32_t faceCount = (uint32_t)mesh.getTriangles().size();
        f.write(reinterpret_cast<const char*>(&faceCount), sizeof(faceCount));

        char buffer[SIZE_OF_FACET] = { 0 };
        for (const auto& tri : mesh.getTriangles())
        {
            float* ptr = ((float*)buffer);
            FPoint3D normal; /* TDB: tri.normal()*/
            // This is the power of C++... delete this comment after reading:)
            *(ptr + 0) = normal.x;
            *(ptr + 1) = normal.y;
            *(ptr + 2) = normal.z;
            ptr += 3;
            *(ptr + 0) = tri.a.x; *(ptr + 1) = tri.a.y; *(ptr + 2) = tri.a.z;
            *(ptr + 3) = tri.b.x; *(ptr + 4) = tri.b.y; *(ptr + 5) = tri.b.z;
            *(ptr + 6) = tri.c.x; *(ptr + 7) = tri.c.y; *(ptr + 8) = tri.c.z;
            f.write(buffer, sizeof(buffer));
        }
        return true;
    }

    bool MeshHandler::readBinary(const std::string& filepath) const
    {
        std::ifstream f(filepath, std::ios_base::binary);
        if (!f.is_open())
            return false;

        f.seekg(0, f.end);
        size_t length = f.tellg();
        f.seekg(0, f.beg);
        size_t actualFaceCount = (length - SIZE_OF_HEADER - sizeof(std::uint32_t)) / SIZE_OF_FACET;

        // skip header
        char header[SIZE_OF_HEADER];
        f.read(header, SIZE_OF_HEADER);

        std::uint32_t expectedFaceCount;
        f.read(reinterpret_cast<char*>(&expectedFaceCount), sizeof(expectedFaceCount));

        if (actualFaceCount != expectedFaceCount)
            trace("File could be corrupted, since actual facet count is not equal to expected facet count");

        beginSolid("");
        char buffer[SIZE_OF_FACET];
        while (actualFaceCount-->0 && f.read(buffer, SIZE_OF_FACET))
        {
            float* ptr = ((float*)buffer);
            FPoint3D normal(*(ptr + 0), *(ptr + 1), *(ptr + 2));
            ptr += 3;
            Triangle3D triangle;
            triangle.a = FPoint3D(*(ptr + 0), *(ptr + 1), *(ptr + 2));
            triangle.b = FPoint3D(*(ptr + 3), *(ptr + 4), *(ptr + 5));
            triangle.c = FPoint3D(*(ptr + 6), *(ptr + 7), *(ptr + 8));
            triangleParsed(triangle);
        }
        endSolid();
        return true;
    }

    bool MeshHandler::readAscii(const std::string& filepath) const
    {
        auto readPoint = [](std::string str, FPoint3D& pt) {
            std::stringstream ss(str);
            std::string tmp;
            int n = 3;
            while (ss && n-->0)
            {
                ss >> tmp;
                float i;
                if (!(std::stringstream(tmp) >> i))
                    return false;
                if (n == 2)
                    pt.x = i;
                else if (n == 1)
                    pt.y = i;
                else if (n == 0)
                    pt.z = i;
                tmp.clear();
            }
            return n == -1;
        };

        auto readTriangle = [&readPoint](std::istream& s, Triangle3D& triangle) {
            std::string tmp;
            int n = 3;
            while (n-->0 && std::getline(s, tmp))
            {
                tmp = trim(tmp);
                FPoint3D vertex;
                auto pos = tmp.find("vertex ");
                if (pos == std::string::npos || !readPoint(tmp.substr(7), vertex))
                    return false;
                if (n == 2)
                    triangle.a = vertex;
                else if (n == 1)
                    triangle.b = vertex;
                else if (n == 0)
                    triangle.c = vertex;
            }
            return n == -1;
        };

        std::ifstream f(filepath);

        size_t lineNum = 0;
        size_t facetNum = 0;

        std::string line;
        while (std::getline(f, line) && ++lineNum)
        {
            line = trim(line);
            if (line.empty()) continue; // empty line is posible (for tabulation), but it's not an error.
            if (line.find("solid ") == 0)
            {
                beginSolid(line.substr(6, line.find(' ', 6) - 6));
            }
            else if (line.find("facet normal ") == 0)
            {
                FPoint3D normal;
                if (!readPoint(line.substr(13), normal))
                    trace(std::string("Error during parsing normal, line ") + std::to_string(lineNum));

                if (!(std::getline(f, line) && ++lineNum) || trim(line).find("outer loop") == std::string::npos)
                    trace(std::string("Error during parsing facet #") + std::to_string(facetNum) + std::string(", line ") + std::to_string(lineNum));

                Triangle3D triangle;
                if (!(readTriangle(f, triangle) && ++facetNum))
                    trace(std::string("Error during parsing facet vertices, facet #") + std::to_string(facetNum) + std::string(", line ") + std::to_string(lineNum));
                triangleParsed(triangle);

                if (!(std::getline(f, line) && ++lineNum) || (trim(line).find("endloop") == std::string::npos))
                    trace(std::string("Error during parsing facet #") + std::to_string(facetNum) + std::string(", line ") + std::to_string(lineNum));
                if (!(std::getline(f, line) && ++lineNum) || (trim(line).find("endfacet") == std::string::npos))
                    trace("Error during parsing facet!");
                continue;
            }
            else if (line.find("endsolid") == 0)
            {
                endSolid();
            }
            else
            {
                // maybe file is corrupted, but skip for now
            }
        }
        return facetNum != 0;
    }
}