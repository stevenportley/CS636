
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


#define SS_MAX_DEPTH 2
#define SS_TOLERANCE 0.15

//#define ADAP_SS
//#define AVG_SS
#define NO_SS

#define BASE_VRES 1024
#define BASE_HRES 1024

#ifdef ADAP_SS
    #define VRES BASE_VRES
    #define HRES BASE_HRES
#endif

#ifdef AVG_SS
    #define VRES BASE_VRES*2
    #define HRES BASE_HRES*2
#endif

#ifdef NO_SS
    #define VRES BASE_VRES
    #define HRES BASE_HRES
#endif


int** ray_count;
int curr_h, curr_v;

void write_file(const char * filename, ColorRGB** image, ssize_t width, ssize_t height);

static auto is_tolerable = [](ColorRGB& A, ColorRGB& B)
{
    ColorRGB diff = A - B;
    float mag = std::sqrt( (diff.red*diff.red) + (diff.green*diff.green) + (diff.blue*diff.blue) );

    return (mag < SS_TOLERANCE) ? true : false;
};



Scene::Scene(float viewplane_distance, Vector3 camera_direction, Vector3 camera_view_up_direction, Vector3 camera_origin)
{
    this->viewplane_distance = viewplane_distance;
    this->camera_direction = camera_direction;
    this->camera_origin = camera_origin;
    this->camera_view_up_directon = camera_view_up_direction;

    normalize(this->camera_direction);

}

void Scene::render() const
{

    /** Camera view angle is 44 degrees **/
    float image_width = 2 * this->viewplane_distance * std::tan(0.383972);
    float image_height = image_width; /** Remember not to do h_res/v_res because of issues with integer division!!! **/

    Vector3 camera_side_view = this->camera_view_up_directon * this->camera_direction;
    Vector3 image_plane_origin = (camera_origin) + (this->viewplane_distance * this->camera_direction) - ( (image_width/2) * camera_side_view) + ((image_height/2) * this->camera_view_up_directon);


    ColorRGB** image = new ColorRGB * [VRES+1]; 
    for(int i = 0; i < VRES+1; i++)
    {
        image[i] = new ColorRGB[HRES+1];

    }


    if(image == NULL)
    {
        std::cout << "Malloc failed\n" << std::endl;
        return;
    }

    Ray this_ray;
    Vector3 prenormal_ray;
    for(int v = 0; v < VRES+1; v++)
    {

        for(int h = 0; h < HRES+1; h++)
        {
            Vector3 horiz_diff = image_width * ((float)h/(((float)HRES)-1)) * camera_side_view;
            Vector3 vert_diff = image_height * ((float)v/(((float)VRES)-1)) * this->camera_view_up_directon;
            Vector3 view_direction = image_plane_origin + horiz_diff - vert_diff - camera_origin;
            normalize(view_direction);

            this_ray = {camera_origin, view_direction};

            std::optional<RayCollision> ray_collision = this->ray_trace(this_ray);
            if(ray_collision)
                image[v][h] = calculate_light(*ray_collision, *this);
            else
                image[v][h] = { }; //Color it black if misses everyting

        }
    }

#ifdef ADAP_SS
    std::cout << "Starting AA-Supersampling" << std::endl;
    ray_count = new int*[VRES];
    for(int i = 0; i < VRES; i++)
        ray_count[i] = new int[HRES];

    /** We have already shot 1 ray per pixel already **/
    for(int v = 0; v < VRES; v++)
        for(int h = 0; h < HRES; h++)
            ray_count[v][h] = 1;

    for(int v = 0; v < VRES; v++)
    {
        for(int h = 0; h < HRES; h++)
        {
            Vector3 horiz_diff = image_width * ((float)h/(((float)HRES)-1)) * camera_side_view;
            Vector3 vert_diff = image_height * ((float)v/(((float)VRES)-1)) * this->camera_view_up_directon;
            Vector3 horiz_diff_next = image_width * ((float)(h+1)/(((float)HRES)-1)) * camera_side_view;
            Vector3 vert_diff_next = image_height * ((float)(v+1)/(((float)VRES)-1)) * this->camera_view_up_directon;
  
            Vector3 view_direction = image_plane_origin + horiz_diff - vert_diff - camera_origin;
            normalize(view_direction);

            this_ray = {camera_origin, view_direction};
           
            if( !is_tolerable( image[v][h], image[v][h+1]) )
            {
                curr_h = h;
                curr_v = v;
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                continue;
            }
            
            if( !is_tolerable( image[v][h], image[v+1][h]) )
            {
                curr_h = h;
                curr_v = v;
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                continue;
            }       

            if( !is_tolerable( image[v+1][h+1], image[v+1][h]) )
            {
                curr_h = h;
                curr_v = v;
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                continue;
            }       

            if( !is_tolerable( image[v+1][h+1], image[v][h+1]) )
            {
                curr_h = h;
                curr_v = v;
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                image[v][h] = adap_ss_ray_trace(image_plane_origin, horiz_diff, horiz_diff_next, vert_diff, vert_diff_next);
                continue;
            }

        }
    }

    ColorRGB** new_image = new ColorRGB * [VRES]; 
    for(int i = 0; i < VRES; i++)
        new_image[i] = new ColorRGB[HRES];

    int max_num_rays = ray_count[0][0];

    for(int v = 0; v < VRES; v++)
        for(int h = 0; h < HRES; h++)
            max_num_rays = (ray_count[v][h] > max_num_rays) ? ray_count[v][h] : max_num_rays;

 
    for(int v = 0; v < VRES; v++)
        for(int h = 0; h < HRES; h++)
        {
            float num_rays = ((float)ray_count[v][h]) / max_num_rays;
            ColorRGB color = { num_rays, num_rays, num_rays };
            new_image[v][h] = color;
        }

    write_file("ray_map.ppm", new_image, HRES, VRES);
    

#endif
    std::cout << "Camera origin: " << camera_origin << std::endl;
    std::cout << "Camera view direction: " << this->camera_direction << std::endl;
    std::cout << "Camera view up: " << this->camera_view_up_directon << std::endl;
    std::cout << "Camera view side: " << camera_side_view << std::endl;


#ifdef AVG_SS
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

    write_file("image.ppm", new_image, HRES/2, VRES/2);
    return;
#endif

    write_file("image.ppm", image, HRES, VRES);


}


ColorRGB Scene::adap_helper( std::optional<ColorRGB> ss_matrix[5][5], Ray vec_matrix[5][5], int depth, int i, int j, int offset) const
{
    
    if( !ss_matrix[i][j])
    {
        std::optional<RayCollision> ray_collision = this->ray_trace( vec_matrix[i][j]); 
        if(ray_collision)
        {
            ss_matrix[i][j] = calculate_light(*ray_collision, *this);
            ray_count[curr_v][curr_h] += 1;
        }
    }

    if( !ss_matrix[i][j+offset])
    {
        std::optional<RayCollision> ray_collision = this->ray_trace( vec_matrix[i][j+offset]);
        if(ray_collision)
        {
            ss_matrix[i][j+offset] = calculate_light(*ray_collision, *this);
            ray_count[curr_v][curr_h] += 1;
        }
    }

    if( !ss_matrix[i+offset][j])
    {
        std::optional<RayCollision> ray_collision = this->ray_trace( vec_matrix[i+offset][j]);
        if(ray_collision)
        {
            ss_matrix[i+offset][j] = calculate_light(*ray_collision, *this);
            ray_count[curr_v][curr_h] += 1;
        }
    }

    if( !ss_matrix[i+offset][j+offset])
    {
        std::optional<RayCollision> ray_collision = this->ray_trace( vec_matrix[i+offset][j+offset]);
        if(ray_collision)
        {
            ss_matrix[i+offset][j+offset] = calculate_light(*ray_collision, *this);
            ray_count[curr_v][curr_h] += 1;
        }
    }

    if( offset == 1)
    {
        ColorRGB top_left = *ss_matrix[i][j];
        ColorRGB top_right = *ss_matrix[i+offset][j];
        ColorRGB bottom_left = *ss_matrix[i][j+offset];
        ColorRGB bottom_right = *ss_matrix[i+offset][j+offset];

        return (top_left + top_right + bottom_left + bottom_right) / 4;
    }


    
    if( !is_tolerable( *ss_matrix[i][j], *ss_matrix[i][j+offset] ))
    {
        int half_off = offset / 2;
        ColorRGB sub_div1 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j,            half_off);
        ColorRGB sub_div2 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j,            half_off);
        ColorRGB sub_div3 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j + half_off, half_off);
        ColorRGB sub_div4 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j + half_off, half_off);
        return (sub_div1 + sub_div2 + sub_div3 + sub_div4) / 4;

    }
    
    if( !is_tolerable( *ss_matrix[i][j], *ss_matrix[i+offset][j] ))
    {
        int half_off = offset / 2;
        ColorRGB sub_div1 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j,            half_off);
        ColorRGB sub_div2 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j,            half_off);
        ColorRGB sub_div3 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j + half_off, half_off);
        ColorRGB sub_div4 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j + half_off, half_off);
        return (sub_div1 + sub_div2 + sub_div3 + sub_div4) / 4;

    }

    
    if( !is_tolerable( *ss_matrix[i+offset][j], *ss_matrix[i+offset][j+offset] ))
    {
        int half_off = offset / 2;
        ColorRGB sub_div1 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j,            half_off);
        ColorRGB sub_div2 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j,            half_off);
        ColorRGB sub_div3 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j + half_off, half_off);
        ColorRGB sub_div4 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j + half_off, half_off);
        return (sub_div1 + sub_div2 + sub_div3 + sub_div4) / 4;
    }

    
    if( !is_tolerable( *ss_matrix[i][j+offset], *ss_matrix[i+offset][j+offset] ))
    {
        int half_off = offset / 2;
        ColorRGB sub_div1 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j,            half_off);
        ColorRGB sub_div2 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j,            half_off);
        ColorRGB sub_div3 = adap_helper( ss_matrix, vec_matrix, depth+1, i,            j + half_off, half_off);
        ColorRGB sub_div4 = adap_helper( ss_matrix, vec_matrix, depth+1, i + half_off, j + half_off, half_off);
        return (sub_div1 + sub_div2 + sub_div3 + sub_div4) / 4;

    }

    ColorRGB top_left = *ss_matrix[i][j];
    ColorRGB top_right = *ss_matrix[i+offset][j];
    ColorRGB bottom_left = *ss_matrix[i][j+offset];
    ColorRGB bottom_right = *ss_matrix[i+offset][j+offset];

    return (top_left + top_right + bottom_left + bottom_right) / 4;
}



ColorRGB Scene::adap_ss_ray_trace(Vector3 image_plane_origin, Vector3 horiz_diff, Vector3 horiz_diff_next, Vector3 vert_diff, Vector3 vert_diff_next) const
{
    std::optional<ColorRGB> ss_matrix[5][5];
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            ss_matrix[i][j] = std::optional<ColorRGB>();
        }
    }


    /** precompute all of the rays **/
    Ray vec_matrix[5][5]; 
    Vector3 horiz_delta = (horiz_diff_next - horiz_diff) / 5;
    Vector3 vert_delta = (vert_diff_next - vert_diff) / 5;

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            Vector3 temp_horiz_diff = horiz_diff + ( j * horiz_delta );
            Vector3 temp_vert_diff = vert_diff + ( i * vert_delta );
            Vector3 view_direction = image_plane_origin + temp_horiz_diff - temp_vert_diff - camera_origin;
            normalize(view_direction);
            vec_matrix[i][j] = { camera_origin, view_direction };
        }
    }

    ColorRGB result = adap_helper( ss_matrix, vec_matrix, 0, 0, 0, 4);
    return result;

}

void Scene::add_model(Model* new_model)
{
    this->models.push_back(new_model);
}

void Scene::add_light(LightSource& new_light)
{
    this->light_sources.push_back(new_light);
}

std::optional<RayCollision> Scene::ray_trace(const Ray& ray) const
{
    std::vector<RayCollision> intersections;

    /** Test this ray with every model and see if there is any intersections (may be more than 1) **/
    for(auto& m : this->models)
    {
        std::optional<RayCollision> this_collision = m->ray_intersect(ray);
        if(this_collision)
        {
            intersections.push_back(*this_collision);
        }
    }

    if(intersections.size() == 0)
        return std::optional<RayCollision>(); /** return black if ray misses **/

    int idx = 0;
    for(int i = 1; i < intersections.size(); i++)
    {
        RayCollision test = intersections[i];
        float closest_dist = magnitude(intersections[idx].location - intersections[idx].source_location);
        float new_dist = magnitude(intersections[i].location - intersections[i].source_location);

        if( new_dist < closest_dist)
        {
            idx = i;
        }

    }

    return std::optional<RayCollision>(intersections[idx]);

}



void write_file(const char* filename, ColorRGB** image, ssize_t width, ssize_t height)
{

    FILE *imageFile;

    imageFile = fopen(filename, "wb");
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
