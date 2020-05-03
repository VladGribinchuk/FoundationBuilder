#include "UnitTests.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <new>
#include <type_traits>
#include <vector>
#include <numeric>

static int __counter = 0;
static typename std::aligned_storage<sizeof(UnitTests), alignof (UnitTests)>::type memoryBufForUT;
UnitTests& globUnitTests = reinterpret_cast<UnitTests&> (memoryBufForUT);
UnitTestsInitializer::UnitTestsInitializer(){ if (__counter++ == 0) new (&globUnitTests) UnitTests(); }
UnitTestsInitializer::~UnitTestsInitializer(){ if (--__counter == 0) (&globUnitTests)->~UnitTests(); }

struct Align
{
    Align(std::string str, int maxChars) 
        : str(str), maxChars(maxChars)
    {}

    std::string operator()()
    {
        int length = static_cast<int>(str.length());
        if (length > maxChars && length > 3)
        {
            str = str.substr(0, maxChars);
            std::for_each(str.begin() + maxChars - 3, str.end(), [](char& c) {c = '.'; });
        }
        else if (length < maxChars)
        {
            str.append(maxChars - length, ' ');
        }
        return str;
    }

    std::string str;
    int maxChars;
};

void UnitTests::run(std::ostream& stream)
{
    if (testCases.empty())
        return;

    // Reset internal states of all test cases before running.
    for (auto& test : testCases)
        test.second->reset();

    std::stringstream bufStream;

    std::vector<double> totalt;
    std::size_t failed = 0;
    std::size_t num = 1;

    bufStream << "[ # ] [Test case name]    [Result] [Time elapsed]\n";
    for (auto& testPair : testCases)
    {
        std::string start;
        start += "[";
        start += std::to_string(num++);
        start += "]";
        bufStream << Align(start, 5)() << " " << Align(testPair.first, 20)();
        try {
            auto t1 = std::chrono::high_resolution_clock::now();
            testPair.second->run();
            auto t2 = std::chrono::high_resolution_clock::now();
            double ms = (double)std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
            totalt.push_back(ms);
            bufStream << "[  OK  ]: " << ms << "ms";
            bufStream << (testPair.second->hasAnyCheck() ? "" : ", but test has no any checks.") << '\n';
        }
        catch (const TestFailure& failure)
        {
            ++failed;
            bufStream << "[FAILED]: " << failure.msg << "\n";
        }
        if (testPair.second->hasOutput())
        {
            auto s = testPair.second->getOutput();
            while (!s.empty())
            {
                bufStream << "      > "; // padding
                auto pos = s.find('\n');
                bufStream << s.substr(0, pos) << '\n';
                s.erase(0, pos==std::string::npos?pos:pos+1);
            }
        }
    }
    bufStream << "                            Total : " << std::accumulate(totalt.begin(), totalt.end(), 0.0) << "ms.\n";
    bufStream << "Resume: " << (failed ? std::to_string(failed) + "/" + std::to_string(testCases.size()) + " test(s) FAILED, see above." :
        "all tests have passed SUCCESSFULLY.")
        << "\n";

    stream << bufStream.str();
}
