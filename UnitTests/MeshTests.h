#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Mesh.h"

using namespace geom_utils;


DEFINE_TEST_CASE(MeshAdd)
{
    Mesh mesh;
    mesh.add({ {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} });

    TEST_ASSERT(mesh[0].a.x == 1 && mesh[0].a.y == 1 && mesh[0].a.z == 0 &&
        mesh[0].b.x == 0 && mesh[0].b.y == 1 && mesh[0].b.z == 0 &&
        mesh[0].c.x == 0 && mesh[0].c.y == 1 && mesh[0].c.z == -1
        , "The mesh must be { {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} }!");
}


DEFINE_TEST_CASE(MeshReadASCII)
{
    std::string stl_text = "solid square_unit_test\n facet normal 0 -1 0\n  outer loop\n  vertex 1 1 0\n  vertex 0 1 0\n  vertex 0 1 -1\n  endloop\n endfacet\n facet normal 0 -1 0\n  outer loop\n  vertex 0 1 -1\n  vertex 1 1 -1\n  vertex 1 1 0\n  endloop\n endfacet\n\nendsolid square_unit_test";
    std::ofstream f("../test_models/square_unit_test_ascii.stl");
    if (f.is_open())
        f << stl_text;
    f.close();

    Mesh mesh;
    mesh.read("../test_models/square_unit_test_ascii.stl");
    std::remove("../test_models/square_unit_test_ascii.stl");

    TEST_ASSERT((mesh.getModelName() == "square_unit_test" && mesh.size() == 2 &&
        mesh[0].a.x == 1 && mesh[0].a.y == 1 && mesh[0].a.z == 0 &&
        mesh[0].b.x == 0 && mesh[0].b.y == 1 && mesh[0].b.z == 0 &&
        mesh[0].c.x == 0 && mesh[0].c.y == 1 && mesh[0].c.z == -1 &&

        mesh[1].a.x == 0 && mesh[1].a.y == 1 && mesh[1].a.z == -1 &&
        mesh[1].b.x == 1 && mesh[1].b.y == 1 && mesh[1].b.z == -1 &&
        mesh[1].c.x == 1 && mesh[1].c.y == 1 && mesh[1].c.z == 0
        ), "The read mesh is wrong!");
}


DEFINE_TEST_CASE(MeshReadBinary)
{
    char headerInfo[80] = "";
    unsigned long nTriLong = 2;
    char attribute[2] = "0";
    std::vector<Triangle3D> facets{ { {1, 1, 0 }, { 0, 1, 0 }, { 0, 1, -1 } }, { {0, 1, -1 }, { 1, 1, -1 }, { 1, 1, 0 } } };

    std::ofstream f("../test_models/square_unit_test_binary.stl", std::ios::binary);
    if (f.is_open())
    {
        f.write(headerInfo, sizeof(headerInfo));
        f.write((char*)&nTriLong, 4);
        char buffer[50] = { 0 };
        for (const auto& tri : facets)
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
            f.write(buffer, sizeof(buffer));
        }
    }
    f.close();


    Mesh mesh;
    mesh.read("../test_models/square_unit_test_binary.stl");
    std::remove("../test_models/square_unit_test_binary.stl");

    TEST_ASSERT((mesh.size() == 2 &&
        mesh[0].a.x == 1 && mesh[0].a.y == 1 && mesh[0].a.z == 0 &&
        mesh[0].b.x == 0 && mesh[0].b.y == 1 && mesh[0].b.z == 0 &&
        mesh[0].c.x == 0 && mesh[0].c.y == 1 && mesh[0].c.z == -1 &&

        mesh[1].a.x == 0 && mesh[1].a.y == 1 && mesh[1].a.z == -1 &&
        mesh[1].b.x == 1 && mesh[1].b.y == 1 && mesh[1].b.z == -1 &&
        mesh[1].c.x == 1 && mesh[1].c.y == 1 && mesh[1].c.z == 0
        ), "The read mesh is wrong!");
}


DEFINE_TEST_CASE(MeshWriteASCII)
{
    Mesh mesh;
    mesh.add({ {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} });
    mesh.setModelName("square_unit_test");
	mesh.writeASCII("../test_models/square_unit_WriteTest_ascii.stl");


	std::ifstream f("../test_models/square_unit_WriteTest_ascii.stl", std::ios::in);
	std::stringstream ss;
	ss << f.rdbuf();
	f.close();
    std::remove("../test_models/square_unit_WriteTest_ascii.stl");

	TEST_CHECK((ss).str(), 
        std::string("solid square_unit_test\n facet normal -0 -1 0\n  outer loop\n  vertex 1 1 0\n  vertex 0 1 0\n  vertex 0 1 -1\n  endloop\n endfacet\n\nendsolid square_unit_test"), 
        "The written mesh is wrong!");
}


DEFINE_TEST_CASE(MeshWriteBinary)
{
    Mesh mesh;
    mesh.add({ {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} });
    mesh.setModelName("square_unit_test");
    mesh.writeBinary("../test_models/square_unit_WriteTest_binary.stl");

    mesh.read("../test_models/square_unit_WriteTest_binary.stl");
    std::remove("../test_models/square_unit_WriteTest_binary.stl");

    TEST_ASSERT((mesh.size() == 1 &&
        mesh[0].a.x == 1 && mesh[0].a.y == 1 && mesh[0].a.z == 0 &&
        mesh[0].b.x == 0 && mesh[0].b.y == 1 && mesh[0].b.z == 0 &&
        mesh[0].c.x == 0 && mesh[0].c.y == 1 && mesh[0].c.z == -1
        ), "The written mesh is wrong!");
}

DEFINE_TEST_CASE(FoundationCreationFacetsModel)
{
    Mesh mesh;
    mesh.read("../test_models/facets_155314.stl");
    Mesh output_mesh = createFoundation(mesh, 6.00);
    output_mesh.writeBinary("../test_models/facets_foundation.stl");
}

DEFINE_TEST_CASE(FoundationCreationConcativeOutline)
{
    Mesh mesh;
    mesh.read("../test_models/concave_outline_binary.stl");
    Mesh output_mesh = createFoundation(mesh, 7.00);
    output_mesh.writeBinary("../test_models/concative_foundation.stl");
}

DEFINE_TEST_CASE(FoundationCreationCube)
{
    Mesh mesh;
    mesh.read("../test_models/cube_20x20x20_ascii.stl");
    Mesh output_mesh = createFoundation(mesh, 8.00);
    output_mesh.writeBinary("../test_models/cube_foundation.stl");
}