#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Triangle.h"
#include "../GeometryCore/include/Mesh.h"
#include "../GeometryCore/include/AABB.h"
#include "../GeometryCore/include/Point.h"
#include <string>
#include <fstream>


//function for copying files
inline void copyFile(const std::string from, const std::string to)
{
    std::ifstream src(from, std::ios::binary);
    std::ofstream dest(to, std::ios::binary);
    dest << src.rdbuf();
};


DEFINE_TEST_CASE(ApplicationRun)
{
    //checks whether file is existing
    auto isFileExist = [](std::string file) -> bool { return !!std::ifstream(file); };

    //copy the executor to folder with tests
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");

    //check whether help works correct
    system("FoundationBuilder.exe -help > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadata.json"), "Application must not create any files if help is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with random data
    system("FoundationBuilder.exe some random data > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadata.json"), "Application must not create any files if random data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with wrong data
    system("FoundationBuilder.exe -i blabla.txt -o alsoBlabla.txt -h -2 > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadata.json"), "Application must not create any files if wrong data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works without data
    system("FoundationBuilder.exe > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadata.json"), "Application must not create any files if data is not stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with correct data (foundation is integrated)
    system("FoundationBuilder.exe -i ../test_models/complex_scene_ascii.stl -o ../test_models/outputModel.stl -h 15 -a -b > ApplicationRun_test.txt");
    TEST_ASSERT(isFileExist("../test_models/outputModel.stl"), "The output model must be created!");                                      //whether file is created
    Mesh foundationWithModel;
    foundationWithModel.read("../test_models/outputModel.stl");
    AABB3D aabb(FPoint3D(-13.5, -13.5, -13.5), FPoint3D(66, 66, 19));
    for (auto i : foundationWithModel.getFacets()) {
        TEST_ASSERT((aabb.contains(i.a) && aabb.contains(i.b) && aabb.contains(i.c)), "All points must be in Axis Aligned Boundary Box"); //whether AABB is correct
    }
    std::remove("ApplicationRun_test.txt");
    std::remove("../test_models/outputModel.stl");

    //check whether generated foundation is correct (foundation is not integrated)
    system("FoundationBuilder.exe -i ../test_models/complex_scene_ascii.stl -o ../test_models/outputModel.stl -h 6 -w 5 -a > ApplicationRun_test.txt");
    TEST_ASSERT(isFileExist("../test_models/outputModel.stl"), "The output model must be created!");                                      //whether file is created
    Mesh foundationMesh;
    foundationMesh.read("../test_models/outputModel.stl");
    AABB3D box(FPoint3D(-1, -1, -0.001), FPoint3D(66, 66, 6.001));
    for (auto i : foundationMesh.getFacets()) {
        TEST_ASSERT((box.contains(i.a) && box.contains(i.b) && box.contains(i.c)), "All points must be in Axis Aligned Boundary Box");    //whether AABB is correct
    }
    std::remove("ApplicationRun_test.txt");
    std::remove("../test_models/outputModel.stl");

    //delete the copied executor
    std::remove("FoundationBuilder.exe");
    std::remove("metadata.json");
}


DEFINE_TEST_CASE(FoundationNormals)
{
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/facets_155314.stl -o ../test_models/outputModel.stl -h 6 -w 5 > unitTestFile.txt");

    Mesh mesh;
    mesh.read("../test_models/outputModel.stl");
    auto facets = mesh.getFacets();
    TEST_ASSERT(facets[0].getNormal() == FPoint3D(0, 0, 1), "Normal of facets on top must be {0, 0, 1}");
    TEST_ASSERT(facets[facets.size() - 1].getNormal() == FPoint3D(0, 0, -1), "Normal of facets on bottom must be {0, 0, -1}");

    std::remove("unitTestFile.txt");
    std::remove("../test_models/outputModel.stl");
    std::remove("FoundationBuilder.exe");
    std::remove("metadata.json");
}


DEFINE_TEST_CASE(FoundationHeight)
{
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/concave_outline_binary.stl -o ../test_models/outputModel.stl -h 7 -w 5 > unitTestFile.txt");

    Mesh mesh;
    mesh.read("../test_models/outputModel.stl");
    auto facets = mesh.getFacets();
    TEST_ASSERT(facets[0].a.z - facets[facets.size() - 1].a.z == 7.00, "Height of foundation must be 7.00");

    std::remove("unitTestFile.txt");
    std::remove("../test_models/outputModel.stl");
    std::remove("FoundationBuilder.exe");
    std::remove("metadata.json");
}


DEFINE_TEST_CASE(FoundationInBox)
{
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/cube_20x20x20_ascii.stl -o ../test_models/outputModel.stl -h 8 -w 5 > unitTestFile.txt");

    Mesh mesh;
    mesh.read("../test_models/outputModel.stl");
    AABB3D box(FPoint3D(-15.001, -15.001, -0.001), FPoint3D(15.001, 15.001, 8.001));
    for (auto i : mesh.getFacets()) {
        TEST_ASSERT((box.contains(i.a) && box.contains(i.b) && box.contains(i.c)), "All points must be in Axis Aligned Boundary Box");
    }

    std::remove("unitTestFile.txt");
    std::remove("../test_models/outputModel.stl");
    std::remove("metadata.json");
    std::remove("FoundationBuilder.exe");
}


DEFINE_TEST_CASE(PlacingFoundationUnderFigure)
{
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/cube_20x20x20_ascii.stl -o ../test_models/outputModel.stl -h 4 -w 5 -b > unitTestFile.txt");

    Mesh model;
    model.read("../test_models/outputModel.stl");
    AABB3D box(FPoint3D(-15.001, -15.001, -5.001), FPoint3D(20.001, 20.001, 20.001));
    for (auto i : model.getFacets()) {
        TEST_ASSERT((box.contains(i.a) && box.contains(i.b) && box.contains(i.c)), "All points must be in Axis Aligned Boundary Box");
    }

    std::remove("unitTestFile.txt");
    std::remove("metadata.json");
    std::remove("../test_models/outputModel.stl");
    std::remove("FoundationBuilder.exe");
}

DEFINE_TEST_CASE(CreatingMetadataFile)
{
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/cube_20x20x20_ascii.stl -o ../test_models/outputModel.stl -h 4 -w 5 -b -m meta.json");
    std::ifstream metadataFile;
    metadataFile.open("meta.json");
    TEST_ASSERT(metadataFile.is_open(), "Metadata file \"meta.json\" must be created");
    metadataFile.close();
    std::remove("FoundationBuilder.exe");
    std::remove("../test_models/outputModel.stl");
    std::remove("meta.json");

    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");
    system("FoundationBuilder.exe -i ../test_models/cube_20x20x20_ascii.stl -o ../test_models/outputModel.stl -h 4 -w 5 -b");
    metadataFile.open("metadata.json");
    TEST_ASSERT(metadataFile.is_open(), "Metadata file with default value \"metadata.json\" must be created");
    metadataFile.close();
    std::remove("FoundationBuilder.exe");
    std::remove("metadata.json");
    std::remove("../test_models/outputModel.stl");
}