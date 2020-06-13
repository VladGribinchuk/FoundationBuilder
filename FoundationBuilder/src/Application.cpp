#include "../include/Application.h"
#include "../../GeometryCore/include/Mesh.h"
#include "../include/FoundationFunctions.h"

// Initialize with command line arguments
void Application::init(int argc, char** argv)
{
    CommandLine cl;
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

    inputFilepath = cl.getValueAs<std::string>("InputModel");
    outputFilepath = cl.getValueAs<std::string>("OutputModel");
    metadataFilepath = cl.getValueAs<std::string>("MetaData");
    height = cl.getValueAs<float>("Height");
    isBuildIn = cl.specified("b");
    isASCII = cl.specified("a");
    inflateValue = cl.getValueAs<float>("InflateValue");
}

// Main logic of the application is here
// Return exit code
int Application::run()
{
    if (help) return 0; //help was called, so other fields aren't initialized by user
    if (height <= 0 || inflateValue <= 0 ||
        inputFilepath.find(".stl", inputFilepath.length() - 4) == std::string::npos ||
        outputFilepath.find(".stl", outputFilepath.length() - 4) == std::string::npos ||
        metadataFilepath.find(".json", metadataFilepath.length() - 5) == std::string::npos) return -1; //some wrong input values

    //read the input model
    geom_utils::Mesh inputMesh;
    if (!inputMesh.read(inputFilepath)) return -1;

    //create a foundation for it
    geom_utils::Mesh outputMesh = geom_utils::createFoundation(inputMesh, height, inflateValue);

    //unite the foundation and input figure in one .stl model, if it is stated
    if (isBuildIn)
        outputMesh = geom_utils::integrateFoundationIntoModel(inputMesh, outputMesh);

    //write in ASCII or binary .STL file
    if (isASCII) {
        if (!outputMesh.writeASCII(outputFilepath)) return -1;
    }
    else
        if (!outputMesh.writeBinary(outputFilepath)) return -1;

    //create and write metadata for it
        //method is not written yet


    return 0;
}