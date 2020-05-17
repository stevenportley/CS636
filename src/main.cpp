
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
   //model.display_contents();
    Vector3 camera_direction = {-2.0f, -1.0f, 2.0f};
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
            model_color = { 0.80f, 0.0f, 0.0f };
        if(i == 2)
            model_color = { 0.80f, 0.80f, 0.0f};
        if(i == 3)
            model_color = { 0.00f, 0.00f, 0.80f};


        Mesh mesh(in, model_color);
        if(i == 1)
            mesh.translate( {0.5, 0.5, 0.5} );
        
        std::vector<std::shared_ptr<Model>> triangle_list = mesh.get_triangles();

        

        BoundingVolumeHierarchy bvh(triangle_list, 0, 0);
        std::cout << "Finished generating a BVH" << std::endl;
        hierarchies.push_back(bvh);
    }

    for( auto& hierarchy : hierarchies)
        scene.add_model(&hierarchy);

    Sphere sphere( { -1.5f, 0.0f, 0.0f}, 0.5, {0.80, 0.10, 0.80} );
    Sphere sphere2( { -1.5f, -1.5f, 0.0f}, 0.5, {0.10, 0.80, 0.80} );
    scene.add_model(&sphere);
    scene.add_model(&sphere2);


    /**
    std::vector<Sphere> sphere_list;
    float y = -2.0f;
    for(int i = 0; i < 9; i++)
    {
        float z = -2.0f;
        for(int j = 0; j<9; j++)
        {
            Sphere sphere({3.0f, y, z}, 0.5f, {(y + 2.1f) / 10, (z + 2.1f) / 10,  0.5f - ((y + z) / 10)});
            sphere_list.push_back(sphere);
            z += 0.5f;
        }
        y += 0.5f;
    }

    std::cout << sphere_list.size() << std::endl;

    for( auto& sphere: sphere_list)
    {
        scene.add_model(&sphere);
    }
**/


/**
    Sphere sphere({-3.0f, -1.50f, 1.5f}, 0.50f, {1.0f, 1.0f, 0.0f});
    Sphere sphere2({-3.0f, 0.0f, 2.5f}, 0.50f, {0.80, 0.20, 0.2});
    Sphere sphere3({-3.0f, 1.00f, 1.0f}, 0.50f, {0.20, 0.80, 0.80});
    Sphere sphere4({-3.0f, 0.0f, 1.0f}, 0.50f, {0.20, 0.20, 0.80});
    scene.add_model(&sphere);
    scene.add_model(&sphere2);
    scene.add_model(&sphere3);
    scene.add_model(&sphere4);

    **/
/**
    ColorRGB model_color = {1.0f, 1.0f, 1.0f};
    Mesh mesh(in, model_color);

    scene.add_model( &mesh );
**/
    Vector3 light_location = {-4.5f, 0.0f, 0.0f};
    Vector3 light_location2 = {3.0f, 2.0f, -3.0f};
    LightSource light = {light_location, {0.80, 0.80, 0.80}};
    LightSource light2 = {light_location2, {0.80, 0.80, 0.80}};
    scene.add_light( light );
    scene.add_light( light2 );
    
    std::cout << "Starting render" << std::endl;
    scene.render();

   
    return 0;

}
