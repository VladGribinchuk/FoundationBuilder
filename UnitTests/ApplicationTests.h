#pragma once
#include <windows.h>
#include "UnitTests.h"

DEFINE_TEST_CASE(ApplicationRun)
{
    //copy the executor to folder with tests
    CopyFile(L"../x64/Debug/FoundationBuilder.exe", L"FoundationBuilder.exe", false);
    struct stat buffer; //buffer for checking files

    //check whether help works correct
    system("FoundationBuilder.exe -help > ApplicationRun_test.txt");
    TEST_ASSERT(stat("output.stl", &buffer) == -1 && stat("metadate.json", &buffer) == -1, "Application must not create any files if help is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with random data
    system("FoundationBuilder.exe some random data > ApplicationRun_test.txt");
    TEST_ASSERT(stat("output.stl", &buffer) == -1 && stat("metadate.json", &buffer) == -1, "Application must not create any files if random data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with wrong data
    system("FoundationBuilder.exe -i blabla.txt -o alsoBlabla.txt -h -2 > ApplicationRun_test.txt");
    TEST_ASSERT(stat("output.stl", &buffer) == -1 && stat("metadate.json", &buffer) == -1, "Application must not create any files if wrong data is stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works without data
    system("FoundationBuilder.exe > ApplicationRun_test.txt");
    TEST_ASSERT(stat("output.stl", &buffer) == -1 && stat("metadate.json", &buffer) == -1, "Application must not create any files if data is not stated!");
    std::remove("ApplicationRun_test.txt");

    //check whether program works with correct data
    system("FoundationBuilder.exe -i ../test_models/complex_scene_ascii.stl -o ../test_models/outputModel.stl -h 15 -a -b > ApplicationRun_test.txt");
    TEST_ASSERT(stat("../test_models/outputModel.stl", &buffer) == 0, "The output model must be created!");
    std::remove("ApplicationRun_test.txt");
    std::remove("../test_models/outputModel.stl");

    //delete the copied executor
    std::remove("FoundationBuilder.exe");
}