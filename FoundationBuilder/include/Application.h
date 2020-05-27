#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <iostream>

class ModelHandler
{
public:
    void read() {}
    void process() {}
    void output() {}
};

struct Option
{
    Option(std::string name, std::string descr, std::string key, bool isFlag, std::string defaultValue = "")
        : name(name), descripiton(descr), key(key), value(""), defaultValue(defaultValue), isFlag(isFlag)
    {
        if (isFlag)
            this->defaultValue = (defaultValue.empty() ? "false" : "true"); //unificate value for flag
    }

    std::string name;
    std::string descripiton;
    std::string key;
    std::string value;
    std::string defaultValue;
    bool isFlag;
};

inline std::ostream& operator<<(std::ostream& stream, const Option& option)
{
    stream << "- " << option.name << ": " << option.descripiton << '\n';
    stream << "\tkey [-" << option.key << "]";
    stream << (option.isFlag ? " is flag" : "");
    stream << (!option.defaultValue.empty() ? ", default value = \""+option.defaultValue+"\"" : "");
    stream << '\n';
    return stream;
}

class CommandLine
{
    std::vector<Option> options;
    std::ostream& os;

public:
    CommandLine(std::ostream& ostream = std::cout) // standard console output by default
        : os(ostream) 
    {}

    // Defines available options
    void add(Option opt) { options.push_back(opt); }
    
    // Returns true if input arguments are valid.
    bool parse(int argc, char** argv) 
    {
        std::vector<std::string> args;
        std::transform(argv + 1/*skip first argument, which is program name*/, argv + argc, std::back_inserter(args), [](char* arg) {
            return std::string(arg); 
        });

        for (int i = 0; i < args.size(); ++i)
        {
            auto arg = args[i];
            if (arg.find('-') == 0)
            {
                std::string key = arg.substr(1);
                auto it = std::find_if(options.begin(), options.end(), [key](const Option& opt) {return opt.key == key; });
                if (it == options.end())
                    continue; // unrecognized key, just skip
                else
                {
                    if (it->isFlag)
                        it->value = "true";
                    else
                    {
                        if (++i < args.size())
                            it->value = args[i];
                        else
                        {
                            os << "Value was not specified for argument: " << arg << '\n';
                            return false;
                        }
                    }
                }
            }
            else
            {
                os << "Unspecified argument found: " << arg << '\n';
                return false;
            }
        }
        return true;
    }

    template<typename T>
    T getValueAs(std::string nameOrKey) const
    { 
        auto it = std::find_if(options.begin(), options.end(), [nameOrKey](const Option& opt) {return opt.key == nameOrKey || opt.name == nameOrKey; });
        if (it == options.end())
            throw std::logic_error(nameOrKey + " option was not specified!");
        T val;
        std::stringstream{ it->value.empty() ? it->defaultValue : it->value } >> std::boolalpha >> val;
        return val;
    }

    // Returns true if option with provided name/key was specified by default or through the command line
    bool specified(std::string nameOrKey) const
    {
        auto it = std::find_if(options.begin(), options.end(), [nameOrKey](const Option& opt) {return opt.key == nameOrKey || opt.name == nameOrKey; });
        if (it != options.end())
        {
            if (it->isFlag)
                return (it->value.empty() ? it->defaultValue == "true" : it->value == "true");
            else
                return !it->value.empty() || !it->defaultValue.empty();
        }
        return false;
    }

    void printHelpInfo() const
    {
        os << "Parameters usage:\n";
        for (const auto& opt : options)
            os << opt;
    }
};

inline void commandlineExample()
{
    // Available options are specifed here by add() method.
    // If something should be changed or new options should be added, 
    // it can be simply done by new add() call, without changing logic of parsing, etc.
    CommandLine cl;
    cl.add(Option("InputModel", "specifies file path to input stl", "i", false, "input.stl"));
    cl.add(Option("OutputModel", "specifies file path to output stl", "o", false, "output.stl"));
    cl.add(Option("FlagOption", "this is the flag option", "t", true));
    cl.add(Option("Dimension", "this is the value option", "d", false));
    cl.add(Option("Help", "help info", "h", true));

    // example set
    int argc = 6;
    char* argv[] = {(char*)"exec.exe", (char*)"-i", (char*)"new_path_in.stl", (char*)"-t", (char*)"-d", (char*)"0.5"};

    if (!cl.parse(argc, argv) || cl.specified("h"))
    {
        cl.printHelpInfo();
        return;
    };

    std::string input = cl.getValueAs<std::string>("InputModel");
    float dimension = cl.getValueAs<float>("Dimension");
}

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
