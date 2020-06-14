#pragma once
#include "UnitTests.h"
#include "../GeometryCore/include/Triangle.h"
#include "../GeometryCore/include/Mesh.h"
#include "../GeometryCore/include/AABB.h"
#include "../GeometryCore/include/Point.h"
#include <string>
#include <fstream>


DEFINE_TEST_CASE(ApplicationRun)
{
    //usefull lambdas
    auto copyFile = [](const std::string from, const std::string to)
    {
        std::ifstream src(from, std::ios::binary);
        std::ofstream dest(to, std::ios::binary);
        dest << src.rdbuf();
    };
    auto isFileExist = [](std::string file) -> bool { return !!std::ifstream(file); };

    //copy the executor to folder with tests
    copyFile("../x64/Debug/FoundationBuilder.exe", "FoundationBuilder.exe");

    //check whether help works correct
    system("FoundationBuilder.exe -help > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadate.json"), "Application must not create any files if help is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with random data
    system("FoundationBuilder.exe some random data > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadate.json"), "Application must not create any files if random data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with wrong data
    system("FoundationBuilder.exe -i blabla.txt -o alsoBlabla.txt -h -2 > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadate.json"), "Application must not create any files if wrong data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works without data
    system("FoundationBuilder.exe > ApplicationRun_test.txt");
    TEST_ASSERT(!isFileExist("output.stl") && !isFileExist("metadate.json"), "Application must not create any files if data is not stated!");
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
}