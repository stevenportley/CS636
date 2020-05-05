
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

int main(int argc, char** argv)
{

    if( argc < 2)
    {
        std::cout << "Usage: main <files>\n";
        return -1;
    }
    //model.display_contents();
    Vector3 camera_direction = {-1.0f, -1.0f, -1.0f};
    Vector3 camera_origin = {3.0f, 3.0f, 3.0f};
    Vector3 camera_view_up_direction = {0.0f, 1.0f, 0.0f};
    Scene scene(0.10f, camera_direction, camera_view_up_direction, camera_origin);


    std::vector<Mesh> mesh_list;
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
        Mesh mesh(in, model_color);
        mesh_list.push_back(mesh);
    }

    for(auto& mesh : mesh_list)
    {
        scene.add_model(&mesh);
    }
    


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
    Vector3 light_location = {-3.5, 1.5f, -1.5f};
    Vector3 light_location2 = {3.5, -1.25f, 1.50f};
    LightSource light = {light_location, {0.80, 0.80, 0.80}};
    LightSource light2 = {light_location2, {0.80, 0.80, 0.80}};
    scene.add_light( light );
    scene.add_light( light2 );

    scene.render();

   
    return 0;

}
