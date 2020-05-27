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

class StreamOutput
{
    std::ostream& stream;
public:
    StreamOutput(std::ostream& stream) 
        : stream(stream) 
    {}

    void flush()
    {
        stream.flush();
    }

    void erase(int cnt)
    {
        while (cnt-->0)
            stream << '\b';
    }

    static std::string padding(int cnt, char fill = ' ')
    {
        return std::string(cnt, fill);
    }

    static std::string align(std::string str, int maxChars)
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

    template<typename T>
    StreamOutput& operator<<(T&& t)
    {
        stream << t;
        return *this;
    }
};

void UnitTests::run(std::ostream& stream)
{
    if (testCases.empty())
        return;

    // Reset internal states of all test cases before running.
    for (auto& tests : testCases)
        for (auto& test : tests.second)
            test.second->reset();

    // wrap up strem
    StreamOutput so(stream);

    std::vector<double> totalt;
    std::size_t failed = 0;

    so << "[ # ] [Test case name]         [Result]\n";
    for (auto& testClass : testCases)
    {
        std::size_t num = 1;
        so << StreamOutput::padding(5) << (testClass.first.empty() ? "Default group of test cases:\n" : (testClass.first + std::string(":\n")));
        for (auto& testPair : testClass.second)
        {
            std::string start;
            start += "[" + std::to_string(num++) + "]";
            so << StreamOutput::align(start, 5) << " " << StreamOutput::align(testPair.first, 25);
            try {
                double ms = 0.0;
                if (auto* repTestCase = dynamic_cast<RepeatedTestCase*>(testPair.second))
                {
                    so << "run iteration ";
                    so.flush();
                    auto iters = repTestCase->iterationCnt;
                    while (iters-->0)
                    {
                        auto s = std::to_string(repTestCase->iterationCnt - iters);
                        so << s;
                        so.flush();

                        auto t1 = std::chrono::high_resolution_clock::now();
                        testPair.second->run();
                        auto t2 = std::chrono::high_resolution_clock::now();
                        ms += (double)std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

                        so.erase(s.size());
                        so.flush();
                    }
                    so.erase(14);
                    so << "[  OK  ]: " << (repTestCase->iterationCnt > 1 ? ("average iteration time = ") : "run time = ") << size_t(ms / repTestCase->iterationCnt) << "ms\n";
                    so.flush();
                }
                else
                {
                    auto t1 = std::chrono::high_resolution_clock::now();
                    testPair.second->run();
                    auto t2 = std::chrono::high_resolution_clock::now();
                    ms += (double)std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
                    so << "[  OK  ]" << (testPair.second->hasAnyCheck() ? "" : ": but test has no any checks.") << '\n';
                }
                totalt.push_back(ms);
            }
            catch (const TestFailure& failure)
            {
                ++failed;
                so << "[FAILED]: " << failure.msg << "\n";
            }
            if (testPair.second->hasOutput())
            {
                auto s = testPair.second->getOutput();
                while (!s.empty())
                {
                    so << StreamOutput::padding(6) << "> ";
                    auto pos = s.find('\n');
                    so << s.substr(0, pos) << '\n';
                    s.erase(0, pos == std::string::npos ? pos : pos + 1);
                }
            }
            so.flush();
        }
        so << '\n';
    }
    so << StreamOutput::padding(5) << "Resume:\n";
    so << (failed ? std::to_string(failed) + "/" + std::to_string(testCases.size()) + " test(s) FAILED, see above." :
        "All tests have passed SUCCESSFULLY.")
        << "\nTotal time = " << std::accumulate(totalt.begin(), totalt.end(), 0.0) << "ms.\n";
    so.flush();
}
