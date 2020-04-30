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

    std::vector<double> totalt;
    std::size_t failed = 0;
    for (auto& testPair : testCases)
    {
        stream << "Running " << Align(testPair.first, 20)();
        try {
            auto t1 = std::chrono::high_resolution_clock::now();
            testPair.second->run();
            auto t2 = std::chrono::high_resolution_clock::now();
            double ms = (double)std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
            totalt.push_back(ms);
            stream << "[  OK  ]: " << ms << "ms";
            stream << (testPair.second->hasAnyCheck() ? "" : ", but test has no any checks.") << '\n';
        }
        catch (const TestFailure& failure)
        {
            ++failed;
            stream << "[FAILED]: " << failure.msg << "\n";
        }
    }
    
    stream << "[TOTAL TIME]: " << std::accumulate(totalt.begin(), totalt.end(), 0.0) << "ms.\n";

    stream << "[  RESULT  ]: " << (failed ? std::to_string(failed) + "/" + std::to_string(testCases.size()) + " test(s) FAILED, see above." :
        "all tests have passed SUCCESSFULLY.")
        << "\n";
}
