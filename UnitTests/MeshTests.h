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

    Mesh mesh = MeshHandler::read("../test_models/square_unit_test_ascii.stl");
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
        for (int i = 0; i < 2; i++)
        {
            //normal
            f.write((char*)&facets[i].a.x, 4);
            f.write((char*)&facets[i].a.y, 4);
            f.write((char*)&facets[i].a.z, 4);

            //p1 coordinates
            f.write((char*)&facets[i].a.x, 4);
            f.write((char*)&facets[i].a.y, 4);
            f.write((char*)&facets[i].a.z, 4);

            //p2 coordinates
            f.write((char*)&facets[i].b.x, 4);
            f.write((char*)&facets[i].b.y, 4);
            f.write((char*)&facets[i].b.z, 4);

            //p3 coordinates
            f.write((char*)&facets[i].c.x, 4);
            f.write((char*)&facets[i].c.y, 4);
            f.write((char*)&facets[i].c.z, 4);

            f.write(attribute, 2);
        }
    }
    f.close();




    Mesh mesh = MeshHandler::read("../test_models/square_unit_test_binary.stl");
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


DEFINE_TEST_CASE(MeshWrite)
{
    Mesh mesh;
    mesh.add({ {1, 1, 0 }, {0, 1, 0}, {0, 1, -1} });
    mesh.setModelName("square_unit_test");
	MeshHandler::write("../test_models/square_unit_WriteTest_ascii.stl", mesh);


	std::ifstream f("../test_models/square_unit_WriteTest_ascii.stl", std::ios::in);
	std::stringstream ss;
	ss << f.rdbuf();
	f.close();
    std::remove("../test_models/square_unit_WriteTest_ascii.stl");

	TEST_CHECK((ss).str(), 
        std::string("solid square_unit_test\n facet normal -0 -1 0\n  outer loop\n  vertex 1 1 0\n  vertex 0 1 0\n  vertex 0 1 -1\n  endloop\n endfacet\n\nendsolid square_unit_test"), 
        "The written mesh is wrong!");
}