
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <optional>
#include "scene.h"


Vector3 ray_trace(const Ray& ray);


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
    float camera_distance = .25;
    float image_width = 2 * camera_distance * 1;
    float image_height = image_width; /** Remember not to do h_res/v_res because of issues with integer division!!! **/

    Vector3 camera_origin = { 0, 0, 0};
    Vector3 camera_view_dir = {0, 0, 1};
    Vector3 camera_view_up = { 0, 1, 0};
    Vector3 camera_side_view = camera_view_up * camera_view_dir;

    Vector3 image_plane_origin = (camera_origin) + (camera_distance * camera_view_dir) - ( (image_width/2) * camera_side_view) + ((image_height/2) * camera_view_up);



    /** Create a ray from the camera origin to every pixel **/
    Ray rays[512][512]; 
    bool image[512][512];
    Vector3 prenormal_ray;
    for(int h = 0; h < h_res; h++)
    {
        for(int v = 0; v < v_res; v++)
        {
            Vector3 horiz_diff = image_width * ((float)h/(((float)h_res)-1)) * camera_side_view;
            Vector3 vert_diff = image_height * ((float)v/(((float)v_res)-1)) * camera_view_up;
            Vector3 view_direction = image_plane_origin + horiz_diff - vert_diff - camera_origin;
            normalize(view_direction);

            rays[h][v] = {camera_origin, view_direction};
            if( this->ray_trace(rays[h][v]))
                image[h][v] = true;
            else
                image[h][v] = false;
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




    FILE *imageFile;

    imageFile = fopen("image.pgm", "wb");
    if(imageFile == NULL){
        std::cout << "Failed to open image" << std::endl;
        return;
    }

    fprintf(imageFile, "P5\n");
    fprintf(imageFile, "%d %d\n\n", h_res, v_res);
    fprintf(imageFile, "255\n");

    for(int h = 0; h < h_res; h++)
    {
        for(int v = 0; v < v_res; v++)
        {
            if(image[h][v])
                fputc(150, imageFile);
            else
                fputc(50, imageFile);
        }
    }

    fclose(imageFile);

}

void Scene::add_model(std::unique_ptr<Model> new_model)
{
    this->models.push_back(std::move(new_model));
}

std::optional<Vector3> Scene::ray_trace(const Ray& ray)
{
    std::vector<Vector3> intersections;
    std::optional<Vector3> temp;

    /** Test this ray with every model and see if there is any intersections (maybe more than 1) **/
    for(auto& m : this->models)
    {
        temp = m->ray_intersect(ray);
        if(temp)
            intersections.push_back(*temp);
    }

    for(auto& i : intersections)
    {
        if( magnitude(i) < magnitude(*temp) )
            temp = i;

    }

    return temp;

}