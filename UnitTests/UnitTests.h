#pragma once
#include <iostream>
#include <string>
#include <map>

// Simple class for unit testing.
// For test cases instantiating use macro
//      DEFINE_TEST_CASE( unique_name_of_the_test_case )
//      {
//          body-of-the-test-case
//      }
// 
// Use RUN_ALL_TESTS() to start all defined test cases.
class UnitTests
{
public:
    struct TestFailure { std::string msg; };

    class TestCase
    {
    public:
        virtual void run() = 0;
        
        // Check if actual is equal to expected, if not - report test failure with message msg.
        template<typename T>
        void CHECK(T&& actual, T&& expected, std::string msg = std::string("")) { check = true;  if (actual != expected) throw TestFailure{ msg }; }

        // Check if cond is true, if not - report test failure with message msg.
        void ASSERT(bool cond, std::string msg = std::string("")) { check = true;  if (!cond) throw TestFailure{ msg }; }
    
        bool hasAnyCheck() const { return check; }
    private:
        bool check = false;
    };

    void add(std::string testName, TestCase* test) { testCases[testName] = test; }
    void run(std::ostream& stream = std::cout);

private:
    std::map<std::string, TestCase*> testCases;
};

extern UnitTests& globUnitTests;

// Ensure UnitTests global object (used for fine macros) is initialized before any TestCases global objects.
static struct UnitTestsInitializer {
    UnitTestsInitializer();
    ~UnitTestsInitializer();
} unitTestsInitializer;

#define DEFINE_TEST_CASE(test) \
class test : public UnitTests::TestCase { \
public: \
    test() {globUnitTests.add(#test, this);};\
    void run() override; \
}; \
namespace {test var_##test;}\
void test ::run()

#define RUN_ALL_TESTS() \
do { \
    globUnitTests.run(); \
} while (0)
