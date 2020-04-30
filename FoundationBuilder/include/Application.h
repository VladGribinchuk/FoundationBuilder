#pragma once

#include <string>

class ModelHandler
{
public:
    void read() {}
    void process() {}
    void output() {}
};

class CommandLine
{
public:
    void parse(int argc, char** argv) { }

    template<typename T>
    T getValue(std::string key) { return T{}; }

    bool isSpecified(std::string key) { return {}; }
};

class Application
{
public:
    static Application& getInstance()
    {
        static Application inst;
        return inst;
    }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Initialize with command line arguments
    void init(int argc, char** argv);

    // Main logic of the application is here
    // Return exit code
    int run();

private:
    Application() {}
};
