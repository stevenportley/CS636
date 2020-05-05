
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <optional>
#include <cmath>
#include "model.h"
#include "mesh.h"
#include "scene.h"
#include "light.h"
#include "boundingbox.h"

std::optional<float> ray_trace(const Ray& ray);
void write_file(ColorRGB** image, ssize_t width, ssize_t height);


Scene::Scene(float viewplane_distance, Vector3 camera_direction, Vector3 camera_view_up_direction, Vector3 camera_origin)
{
    this->viewplane_distance = viewplane_distance;
    this->camera_direction = camera_direction;
    this->camera_origin = camera_origin;
    this->camera_view_up_directon = camera_view_up_direction;

    normalize(this->camera_direction);

}

void Scene::render()
{

    /** Camera view angle is 44 degrees **/
    float image_width = 2 * this->viewplane_distance * std::tan(0.383972);
    float image_height = image_width; /** Remember not to do h_res/v_res because of issues with integer division!!! **/

    Vector3 camera_side_view = this->camera_view_up_directon * this->camera_direction;
    Vector3 image_plane_origin = (camera_origin) + (this->viewplane_distance * this->camera_direction) - ( (image_width/2) * camera_side_view) + ((image_height/2) * this->camera_view_up_directon);


    ColorRGB** image = new ColorRGB * [VRES]; 
    for(int i = 0; i < VRES; i++)
    {
        image[i] = new ColorRGB[HRES];

    }


    if(image == NULL)
    {
        std::cout << "Malloc failed\n" << std::endl;
        return;
    }

    Ray this_ray;
    Vector3 prenormal_ray;
    for(int v = 0; v < VRES; v++)
    {
        for(int h = 0; h < HRES; h++)
        {
            Vector3 horiz_diff = image_width * ((float)h/(((float)HRES)-1)) * camera_side_view;
            Vector3 vert_diff = image_height * ((float)v/(((float)VRES)-1)) * this->camera_view_up_directon;
            Vector3 view_direction = image_plane_origin + horiz_diff - vert_diff - camera_origin;
            normalize(view_direction);

            this_ray = {camera_origin, view_direction};

            std::optional<ColorRGB> pixel_color = this->ray_trace(this_ray);

            if( pixel_color)
            {
                image[v][h] = *pixel_color;
            }else{
                image[v][h] = {0.0f, 0.0f, 0.0f};
            }
        }
    }
    std::cout << "Camera origin: " << camera_origin << std::endl;
    std::cout << "Camera view direction: " << this->camera_direction << std::endl;
    std::cout << "Camera view up: " << this->camera_view_up_directon << std::endl;
    std::cout << "Camera view side: " << camera_side_view << std::endl;

    ColorRGB** new_image = new ColorRGB * [VRES/2]; 
    for(int i = 0; i < VRES/2; i++)
    {
        new_image[i] = new ColorRGB[HRES/2];

    }


    for(int i = 0; i < HRES/2; i++)
    {
        for(int j = 0; j < VRES/2; j++)
        {
            int offset_i = 2 * i;
            int offset_j = 2 * j;

            ColorRGB one = image[offset_i][offset_j];
            ColorRGB two = image[offset_i][offset_j+1];
            ColorRGB three = image[offset_i+1][offset_j];
            ColorRGB four = image[offset_i+1][offset_j+1];

            float new_red = (one.red + two.red + three.red + four.red) / 4;
            float new_green = (one.green + two.green + three.green + four.green) / 4;
            float new_blue = (one.blue + two.blue + three.blue + four.blue) / 4;

            new_image[i][j] = {new_red, new_green, new_blue};

        }
    }


    write_file(new_image, HRES/2, VRES/2);


}

void Scene::add_model(Model* new_model)
{
    this->models.push_back(new_model);
}

void Scene::add_light(LightSource& new_light)
{
    this->light_sources.push_back(new_light);
}

std::optional<ColorRGB> Scene::ray_trace(const Ray& ray)
{
    std::vector<RayCollision> intersections;
    std::vector<ColorRGB> colors;
    std::optional<RayCollision> temp;

    /** Test this ray with every model and see if there is any intersections (may be more than 1) **/
    for(auto& m : this->models)
    {
        temp = m->ray_intersect(ray, this->light_sources);
        if(temp)
        {
            intersections.push_back(*temp);
            colors.push_back(temp->color);
        }
    }

    if(intersections.size() == 0)
        return std::optional<ColorRGB>();



    int idx = 0;
    for(int i = 1; i < intersections.size(); i++)
    {
        RayCollision test = intersections[i];
        if(magnitude(intersections[i].location) < magnitude( intersections[idx].location))
        {
            idx = i;
        }


    }

    return colors[idx];

}



void write_file(ColorRGB** image, ssize_t width, ssize_t height)
{

    FILE *imageFile;

    imageFile = fopen("image.ppm", "wb");
    if(imageFile == NULL){
        std::cout << "Failed to open image" << std::endl;
        return;
    }

    fprintf(imageFile, "P3\n%d %d\n255\n", width, height);

    for(int v = 0; v < height; v++)
    {
        for(int h = 0; h < width; h++)
        {
                uint8_t red = (uint8_t) (image[v][h].red*255);
                uint8_t green = (uint8_t) (image[v][h].green*255);
                uint8_t blue = (uint8_t) (image[v][h].blue*255);

                fprintf(imageFile, "%d %d %d\t", red, green, blue);
        }
    
        fprintf(imageFile, "\n");

    }

    fclose(imageFile);

}