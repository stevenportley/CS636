
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "vector3.h"
#include "model.h"
#include "mesh.h"
#include "sphere.h"
#include "scene.h"
#include "triangle.h"
#include "light.h"

int main(int argc, char** argv)
{
   //model.display_contents();
    Vector3 camera_direction = {-1.0f, -1.0f, 0.0f};
    Vector3 camera_origin = {4.0f, 4.0f, 0.0f};
    Vector3 camera_view_up_direction = {0.0f, 1.0f, 0.0f};
    Scene scene(0.10f, camera_direction, camera_view_up_direction, camera_origin);

    ModelProperties default_properties = {
        .color = {0.80f, 0.80f, 0.80f},
        .refr_index = 1.0f,
        .ks = {0.4f, 0.4f, 0.4f},
        .kd = {0.6f, 0.6f, 0.6f},
        .kr = {0.3f, 0.3f, 0.3f},
        .kt = {0.1f, 0.1f, 0.1f},
        .ka = {0.5f, 0.5f, 0.5f}
    };

    std::vector<Mesh> mesh_list;
    ModelProperties mesh_prop = default_properties;

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
            mesh_prop.color = { 0.90f, 0.90f, 0.90f};

        if(i == 3)
            mesh_prop.color = { 0.8f, 0.2f, 0.20f};

        Mesh mesh(in, mesh_prop);

        if(i == 2)
            mesh.translate( {0.0, 0.0, 0.0} );

        if(i == 3)
            mesh.translate( {0.0, 1.0, 1.0} );

        if(i == 4)
            mesh.translate( {0.0, 0.5, -1.0} );

        if(i == 5)
            mesh.translate( {-1.5, 1.0, 1.0} );

        mesh_list.push_back(mesh);

    }


    for(auto& mesh: mesh_list)
    {
        scene.add_model(&mesh);
    }

    ModelProperties clear = default_properties;
    clear.refr_index = 1.52f;
    clear.kt = {0.99f, 0.99f, 0.99f};
    clear.kr = {0.1f, 0.1f, 0.1f};
    clear.kd = {0.1f, 0.1f, 0.1f};
    clear.ka = {0.1f, 0.1f, 0.1f};
    clear.ks = {0.1f, 0.1f, 0.1f};

    ModelProperties s2 = default_properties;
    ModelProperties s3 = default_properties;
    s2.color = { 0.80f, 0.20f, 0.20f};
    s3.color = { 0.20f, 0.80f, 0.20f};

    
    Sphere sphere( {1.5f, 1.5f, 0.0f}, 0.35, clear );
    Sphere sphere2( {0.25f, 0.15f, -0.25f}, 0.15, default_properties);
    Sphere sphere3( {0.60f, 0.15f, 0.20f}, 0.15, default_properties);
    Sphere sphere4( {0.0f, 0.2f, -0.5f}, 0.15, default_properties);
    Sphere sphere5( {0.3f, 0.2f, 0.0f}, 0.15, default_properties);
    Sphere sphere6( {-0.2f, 0.3f, 0.15f}, 0.15, default_properties);
    Sphere sphere7( {0.4f, 0.15f, 0.5f}, 0.15, s3);
    Sphere sphere8( {-0.4f, 0.4f, 0.5f}, 0.15, s3);
    Sphere sphere9( {0.0f, 0.55f, -0.10f}, 0.15, s2);
    
    scene.add_model(&sphere);
/**
    scene.add_model(&sphere2);
    scene.add_model(&sphere3);
    scene.add_model(&sphere4);
    scene.add_model(&sphere5);
    scene.add_model(&sphere6);
**/
    scene.add_model(&sphere7);
    scene.add_model(&sphere8);
    scene.add_model(&sphere9);

    Vector3 light_location = {5.0f, 5.0f, 5.0f};
    LightSource light = {light_location, {0.80, 0.80, 0.80}};
    Vector3 light_location2 = {-5.0f, 5.0f, -5.0f};
    LightSource light2 = {light_location2, {0.80, 0.80, 0.80}};
    scene.add_light( light );
    scene.add_light( light2 );
    
    std::cout << "Starting render" << std::endl;
    scene.render();
   
    return 0;

}
