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
    Mesh mesh = MeshHandler::read("../test_models/square_unit_test_ascii.stl");
    TEST_ASSERT((mesh.getComment() == "square_unit_test" && mesh.size() == 2 &&
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
    Mesh mesh = MeshHandler::read("../test_models/square_unit_test_binary.stl");
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
    mesh.setComment("square_unit_test");
	MeshHandler::write("../test_models/square_unit_WriteTest_ascii.stl", mesh);


	std::ifstream f("../test_models/square_unit_WriteTest_ascii.stl", std::ios::in);
	std::stringstream ss;
	ss << f.rdbuf();
	f.close();

	TEST_CHECK((ss).str(), 
        std::string("solid square_unit_test\n facet normal 0 0 1\n  outer loop\n  vertex 1 1 0\n  vertex 0 1 0\n  vertex 0 1 -1\n  endloop\n endfacet\n\nendsolid square_unit_test"), 
        "The written mesh is wrong!");
}