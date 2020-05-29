
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
   //model.display_contents();
    Vector3 camera_direction = {-1.0f, 0.0f, 0.0f};
    Vector3 camera_origin = {4.0f, 0.0f, 0.0f};
    Vector3 camera_view_up_direction = {0.0f, 1.0f, 0.0f};
    Scene scene(0.10f, camera_direction, camera_view_up_direction, camera_origin);


    ColorRGB model_color = {1.0f, 1.0f, 1.0f};
    std::vector<Mesh> mesh_list;

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
            model_color = { 0.80f, 0.80f, 0.80f };
        if(i == 2)
            model_color = { 0.80f, 0.80f, 0.0f};
        if(i == 3)
            model_color = { 0.80f, 0.80f, 0.80f};
        if(i == 4)
            model_color = { 0.80f, 0.00f, 0.80f};

        Mesh mesh(in, model_color);
/**
        if(i == 1)
            mesh.translate( {0.0, 0.0, 0.0} );
        
        if(i == 2)
            mesh.translate( {-0.5, 1.0, 0.0} );

        if(i == 3)
            mesh.translate( {-1.0, 1.0, -1.0} );

        if(i == 4)
            mesh.translate( {0.0, 1.5, 0.0} );
**/

        std::cout << "Adding mesh to list " << std::endl;

        mesh_list.push_back(mesh);

    }


    for(auto& mesh: mesh_list)
    {
        scene.add_model(&mesh);
    }
    
/**
    Sphere sphere( {0.0f, 0.5f, 0.50f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere2( {0.0f, 1.0f, 0.50f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere3( {0.0f, 1.5f, 0.50f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere4( {0.0f, 0.0f, -0.5f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere5( {0.0f, 0.0f, 0.5f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere6( {0.0f, -0.5f, -0.5f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere7( {0.0f, 0.5f, 0.5f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere8( {0.0f, -0.5f, 0.5f}, 0.15, {0.80f, 0.80f, 0.80f} );
    Sphere sphere9( {0.0f, 0.5f, -0.50f}, 0.15, {0.80f, 0.80f, 0.80f} );
    
    scene.add_model(&sphere);
    scene.add_model(&sphere2);
    scene.add_model(&sphere3);
    scene.add_model(&sphere4);
    scene.add_model(&sphere5);
    scene.add_model(&sphere6);
    scene.add_model(&sphere7);
    scene.add_model(&sphere8);
    scene.add_model(&sphere9);

**/
    Vector3 light_location = {5.0f, -2.0f, 0.0f};
    LightSource light = {light_location, {0.80, 0.80, 0.80}};
    scene.add_light( light );
    
    std::cout << "Starting render" << std::endl;
    scene.render();

   
    return 0;

}
