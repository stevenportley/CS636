

#include "scene.h"

Scene::Scene(int viewplane_distance, Vector3 camera_direction, Vector3 camera_origin)
{
    this->viewplane_distance = viewplane_distance;
    this->camera_direction = camera_direction;
    this->camera_origin = camera_origin;

}

void Scene::render()
{

    /** Camera view angle is 90 degrees **/
    float camera_distance = 1.0;
    float image_width = 2 * camera_distance * 1;


    Vector3 image_plane_origin = { 1, 1, 1};
    Vector3 camera_origin = { 0, 0, 0};

    int resolution_x = 512;
    int resolution_y = 512;

    /** Create a ray from the origin to every pixel **/
    Vector3 rays[512][512]; 
    Vector3 prenormal_ray;
    for(int h = 0; h < resolution_x; h++)
    {
        for(int v = 0; v < resolution_y; v++)
        {
            prenormal_ray = ( image_plane_origin + image_width * (h-1)) * ) 
        }

    }

}
    