
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

    //model.display_contents();

    Scene scene(1, {1, 1, 1}, {0, 0, 0});

    scene.add_model( std::make_unique<Sphere>( Sphere({1, 1, 1}, 0.50)));

    Triangle t1( {-1.0f, 2.5f, 1.0f}, {-1.5f, 2.0f, 1.0f}, {-1.0f, 1.5f, 1.0f});
    scene.add_model( std::make_unique<Triangle>(t1));
    
    scene.add_model( std::make_unique<Mesh>(in));

    scene.render();
   
    return 0;

}
