
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "model.h"
#include "mesh.h"
#include "sphere.h"
#include "scene.h"
#include "triangle.h"
#include "light.h"
#include "boundingvolumehierarchy.h"

int main(int argc, char** argv)
{

    if( argc < 2)
    {
        std::cout << "Usage: main <files>\n";
        return -1;
    }
    //model.display_contents();
    Vector3 camera_direction = {-1.0f, -0.5f, 1.0f};
    Vector3 camera_origin = {2.0f, 1.0f, -2.0f};
    Vector3 camera_view_up_direction = {0.0f, 1.0f, 0.0f};
    Scene scene(0.10f, camera_direction, camera_view_up_direction, camera_origin);


    std::vector<BoundingVolumeHierarchy> hierarchies;
    ColorRGB model_color = {1.0f, 1.0f, 1.0f};

    for(int i = 1; i < argc; i++)
    {
        printf("Reading model: %s\n", argv[i]);
        std::ifstream in(argv[i]);
        if(!in)
        {
            printf("Error reading %s\n", argv[i]);
            continue;
        }

        if(i == 1)
            model_color = { 0.25, 0.50, 0.75 };

        if(i == 2)
            model_color = {0.75, 0.50, 0.25 };

        if(i == 3)
            model_color = { 0.25, 0.50, 0.25 };


        Mesh mesh(in, model_color);

        if(i == 1)
            mesh.translate( {0.50, 0.50, 0.50});

        std::vector<std::shared_ptr<Model>> triangle_list = mesh.get_triangles();
        

        BoundingVolumeHierarchy bvh(triangle_list, 0, 0);
        std::cout << "Finished generating a BVH" << std::endl;
        hierarchies.push_back(bvh);
    }

    for( auto& hierarchy : hierarchies)
        scene.add_model(&hierarchy);


    //Sphere sphere({3.0f, 1.5f, 1.5f}, 0.5f, {1.0f, 1.0f, 0.0f});
    //Sphere sphere2({0, 1, 0.0}, 0.50, {0.80, 0.20, 0.2});
    //Sphere sphere3({3.0f, -1.5f, -1.5f}, 0.5f, {0.20, 0.80, 0.80});
    //scene.add_model(&sphere);
    //scene.add_model(&sphere2);
    //scene.add_model(&sphere3);
/**
    ColorRGB model_color = {1.0f, 1.0f, 1.0f};
    Mesh mesh(in, model_color);

    scene.add_model( &mesh );
**/
    Vector3 light_location = {3.5, 1.5f, -2.5f};
    LightSource light = {light_location, {0.80, 0.80, 0.80}};
    scene.add_light( light );
    
    std::cout << "Starting render" << std::endl;
    scene.render();

   
    return 0;

}
