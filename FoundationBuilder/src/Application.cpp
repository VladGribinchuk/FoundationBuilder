#include "../include/Application.h"

// Initialize with command line arguments
void Application::init(int argc, char** argv)
{
    CommandLine cl;
    cl.add(Option("InputModel", "specifies file path to input stl", "i", false, "input.stl"));
    cl.add(Option("OutputModel", "specifies file path to output stl", "o", false, "output.stl"));
    cl.add(Option("MetaDate", "specifies file path to output stl", "m", false, "metadate.json"));
    cl.add(Option("Height", "this is the value option", "s", false));
    cl.add(Option("BuildIn", "this is flag option", "b", true));
    cl.add(Option("ASCIIOrBinary", "this is flag option", "f", true));
    cl.add(Option("Help", "help info", "h", true));

    if (!cl.parse(argc, argv) || cl.specified("h"))
    {
        cl.printHelpInfo();
        return;
    };

    std::string input = cl.getValueAs<std::string>("InputModel");
    std::string output = cl.getValueAs<std::string>("OutputModel");
    std::string metadate = cl.getValueAs<std::string>("MetaDate");
    float height = cl.getValueAs<float>("Height");
    bool buildIn = cl.specified("b");
    bool fileType = cl.specified("f");
}

// Main logic of the application is here
// Return exit code
int Application::run()
{
    return 0;
}
