
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "mesh.h"
#include "sphere.h"
#include "scene.h"
#include "triangle.h"

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

    Mesh mesh(in);

    //model.display_contents();

    Scene scene(1, {1, 1, 1}, {0, 0, 0});

    //scene.add_model( std::make_unique<Sphere>( Sphere({0, 0, 1}, 0.50)));

    Triangle t1( {0.5f, 0.0f, 1.0f}, {-0.5f, 0.0f, 1.0f}, {0.0f, 0.5f, 1.0f});
    
    scene.add_model( std::make_unique<Triangle>(t1));


    scene.render();
   
    return 0;

}
