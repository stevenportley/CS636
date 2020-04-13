
#include "scene.h"


Vector3 ray_trace(const Ray& ray, const Model& model);


Scene::Scene(int viewplane_distance, Vector3 camera_direction, Vector3 camera_origin)
{
    this->viewplane_distance = viewplane_distance;
    this->camera_direction = camera_direction;
    this->camera_origin = camera_origin;

}

void Scene::render()
{
    int h_res = 512;
    int v_res = 512;

    /** Camera view angle is 90 degrees **/
    float camera_distance = 1.0;
    float image_width = 2 * camera_distance * 1;
    float image_height = image_width; /** Remember not to do h_res/v_res because of issues with integer division!!! **/

    Vector3 camera_origin = { 0, 0, 0};
    Vector3 camera_view_dir = {0, 0, 1};
    Vector3 camera_view_up = { 0, 1, 0};
    Vector3 camera_side_view = camera_view_up * camera_view_dir;

    Vector3 image_plane_origin = (camera_origin) + (camera_distance * camera_view_dir) - ( (image_width/2) * camera_side_view) + ((image_height/2) * camera_view_up);



    /** Create a ray from the camera origin to every pixel **/
    Ray rays[512][512]; 
    Vector3 prenormal_ray;
    for(int h = 0; h < h_res; h++)
    {
        for(int v = 0; v < v_res; v++)
        {
            Vector3 horiz_diff = image_width * ((float)h/(((float)h_res)-1)) * camera_side_view;
            Vector3 vert_diff = image_height * ((float)v/(((float)v_res)-1)) * camera_view_up;
            Vector3 view_direction = image_plane_origin + horiz_diff - vert_diff - camera_origin;

            rays[h][v] = {camera_origin, view_direction};
        }

    }

    std::cout << "Camera origin: " << camera_origin << std::endl;
    std::cout << "Camera view direction: " << camera_view_dir << std::endl;
    std::cout << "Camera view up: " << camera_view_up << std::endl;
    std::cout << "Camera view side: " << camera_side_view << std::endl;
    std::cout << "Top left image view direction " << rays[0][0].direction << std::endl;
    std::cout << "Top right image view direction " << rays[511][0].direction << std::endl;
    std::cout << "Bottom left image view direction " << rays[0][511].direction << std::endl;
    std::cout << "Bottom right image view direction " << rays[511][511].direction << std::endl;
    std::cout << "Middle image view direction " << rays[255][255].direction << std::endl;

}

Vector3 ray_trace(const Ray& ray, const Model& model)
{


}