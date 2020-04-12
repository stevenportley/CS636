
#include <vector>
#include <iostream>
#include <fstream>

#include "model.h"
#include "scene.h"

int main(int argc, char** argv)
{

    if( argc != 2)
    {
        std::cout << "Usage: main <file>\n";
        return -1;
    }

    std::ifstream in(argv[1]);
    if(!in)
    {
        std::cout << "Reading file failed!\n";
        return -1;
    }

    Model model(in);

    //model.display_contents();

    Scene scene(1, {1, 1, 1}, {0, 0, 0});
    scene.render();
    return 0;

}
