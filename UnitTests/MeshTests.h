#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Mesh.h"

using namespace geom_utils;

DEFINE_TEST_CASE(MeshHandlerReadWrite)
{
    Mesh mesh;
    MeshHandler handler(std::bind(&Mesh::add, &mesh, std::placeholders::_1),
        [this](std::string msg) { TEST_ASSERT(false, "Smth was wrong: " + msg); /*unexpected issue*/ },
        [this](std::string solidName) {TEST_OUTPUT() << "Start parsing: " << solidName << " solid"; },
        [this]() {TEST_OUTPUT() << "...parsing is success"; }
    );

    // MeshHandler methods, providing callbacks
    TEST_ASSERT(handler.read("../test_models/cube_20x20x20_ascii.stl"));
    TEST_ASSERT(!mesh.isEmpty(), "Mesh should not be empty!");
    TEST_CHECK(mesh.getTriangles().size(), (size_t)12, "Cube should contain 12 facets!");
    TEST_ASSERT(handler.write(mesh, "test_mesh_binary.stl"));
    std::remove("test_mesh_binary.stl");
}

DEFINE_TEST_CASE(MeshReadWrite)
{
    Mesh mesh;

    // try direct Mesh calls, withot callbacks
    TEST_ASSERT(mesh.read("../test_models/cube_20x20x20_binary.stl"));
    TEST_ASSERT(!mesh.isEmpty(), "Mesh should not be empty!");
    TEST_CHECK(mesh.getTriangles().size(), (size_t)12, "Cube should contain 12 facets!");
    TEST_ASSERT(mesh.write("test_mesh_binary.stl"));
    std::remove("test_mesh_binary.stl");
}
