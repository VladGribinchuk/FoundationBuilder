#include "../include/Application.h"
#include "../../GeometryCore/include/Mesh.h"
#include "../include/FoundationFunctions.h"

// Initialize with command line arguments
void Application::init(int argc, char** argv)
{
    cl.add(Option("InputModel", "specifies a file path to input .stl", "i", false, "input.stl"));
    cl.add(Option("OutputModel", "specifies a file path to output .stl", "o", false, "output.stl"));
    cl.add(Option("MetaData", "specifies a file path to  metadate .json", "m", false, "metadate.json"));
    cl.add(Option("Height", "specifies the foundation height", "h", false, "1.0"));
    cl.add(Option("BuildIn", "input and output models are in one file, if true", "b", true));
    cl.add(Option("OutputInASCII", "the output file is in ASCII format, if true", "a", true));
    cl.add(Option("Help", "help info", "help", true));
    cl.add(Option("InflateValue", "specifies how much wider will be foundation than input stl", "w", false, "5.0"));

    if (!cl.parse(argc, argv) || cl.specified("help"))
    {
        cl.printHelpInfo();
        help = true;
        return;
    };

}

// Main logic of the application is here
// Return exit code
int Application::run()
{
    if (help) return 0; //help was called, so other fields aren't initialized by user
    if (cl.getValueAs<float>("Height") <= 0 || cl.getValueAs<float>("InflateValue") <= 0 ||
        cl.getValueAs<std::string>("InputModel").find(".stl", cl.getValueAs<std::string>("InputModel").length() - 4) == std::string::npos ||
        cl.getValueAs<std::string>("OutputModel").find(".stl", cl.getValueAs<std::string>("OutputModel").length() - 4) == std::string::npos ||
        cl.getValueAs<std::string>("MetaData").find(".json", cl.getValueAs<std::string>("MetaData").length() - 5) == std::string::npos) return -1; //some wrong input values

    //read the input model
    geom_utils::Mesh inputMesh;
    if (!inputMesh.read(cl.getValueAs<std::string>("InputModel"))) return -1;

    //create a foundation for it
    geom_utils::Mesh outputMesh = geom_utils::createFoundation(inputMesh, cl.getValueAs<float>("Height"), cl.getValueAs<float>("InflateValue"));

    //unite the foundation and input figure in one .stl model, if it is stated
    if (cl.specified("b"))
        outputMesh = geom_utils::integrateFoundationIntoModel(inputMesh, outputMesh);

    //write in ASCII or binary .STL file
    if (cl.specified("a")) {
        if (!outputMesh.writeASCII(cl.getValueAs<std::string>("OutputModel"))) return -1;
    }
    else
        if (!outputMesh.writeBinary(cl.getValueAs<std::string>("OutputModel"))) return -1;

    //create and write metadata for it
        //method is not written yet


    return 0;
}