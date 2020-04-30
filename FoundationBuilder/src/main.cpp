#include "../include/Application.h"

int main(int argc, char** argv)
{
    Application::getInstance().init(argc, argv);
    return Application::getInstance().run();
}