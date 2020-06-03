#include "../include/Application.h"

// Initialize with command line arguments
void Application::init(int argc, char** argv)
{
    CommandLine cl;
    cl.add(Option("InputModel", "specifies file path to input stl", "i", false, "input.stl"));
    cl.add(Option("OutputModel", "specifies file path to output stl", "o", false, "output.stl"));
    cl.add(Option("MetaData", "specifies file path to  metadate json", "m", false, "metadate.json"));
    cl.add(Option("Height", "this is the foundation height", "h", false,"1.0"));
    cl.add(Option("BuildIn", "write result in one or two files", "b", true));
    cl.add(Option("OutputInASCII", "write result in Binary or ASCII files", "a", true));
    cl.add(Option("Help", "help info", "help", true));

    if (!cl.parse(argc, argv) || cl.specified("help"))
    {
        cl.printHelpInfo();
        return;
    };

    std::string input = cl.getValueAs<std::string>("InputModel");
    std::string output = cl.getValueAs<std::string>("OutputModel");
    std::string metadate = cl.getValueAs<std::string>("MetaData");
    float height = cl.getValueAs<float>("Height");
    bool buildIn = cl.specified("b");
    bool fileType = cl.specified("a");
}

// Main logic of the application is here
// Return exit code
int Application::run()
{
    return 0;
}
