
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <optional>
#include "vector3.h"
#include "model.h"
#include "mesh.h"
#include "triangle.h"
#include "boundingbox.h"

#define NUM_MODELS_THRESHOLD 10
#define BOUND_MAX_DEPTH 3

static BoundingBox generate_boundingbox(std::vector<Triangle>& triangles);
static void parse_model_file( std::ifstream& model_file, ColorRGB color, std::vector<Triangle>& triangle_list);

Mesh::Mesh(ColorRGB color, std::vector<Triangle> triangles, int current_depth, int sort_axis) : triangles(triangles), color(color), bounding_box({}, {})
{
    std::cout << "Initializing mesh with " << triangles.size() << " triangles" << std::endl;
    this->bounding_box = generate_boundingbox(triangles);
    this->subdivide(current_depth, sort_axis);
  
    std::cout << "Bounding box: [" << this->bounding_box.p1.x << ", " << this->bounding_box.p1.y << ", "  << this->bounding_box.p1.z << "]   [" << this->bounding_box.p2.x << ", " << this->bounding_box.p2.y  << ", " << this->bounding_box.p2.z << "]" << std::endl;
}


Mesh::Mesh(std::ifstream& model_file, ColorRGB color) : color(color), bounding_box({}, {})
{

    parse_model_file(model_file, color, this->triangles);

    std::cout << "Loaded model with " << this->triangles.size() << " trianles" << std::endl;

    this->bounding_box = generate_boundingbox( this->triangles);

    std::cout << "Bounding box: [" << this->bounding_box.p1.x << ", " << this->bounding_box.p1.y << ", "  << this->bounding_box.p1.z << "]   [" << this->bounding_box.p2.x << ", " << this->bounding_box.p2.y  << ", " << this->bounding_box.p2.z << "]" << std::endl;


    this->subdivide(0, 0);
}

BoundingBox Mesh::get_boundingbox()
{
    return this->bounding_box;
}



std::optional<RayCollision> Mesh::ray_intersect( const Ray& ray)
{
    
    std::optional<RayCollision> output;
    std::optional<RayCollision> temp;

    /** Miss bounding box, early drop out **/
    if(!(this->bounding_box.does_intersect(ray)))
    {   
        return std::optional<RayCollision>();
    }

    if(this->sub_hierarchies.size() == 2)
    {
        for(auto& sub_hierarchy : this->sub_hierarchies)
        {
            if( (temp = sub_hierarchy.ray_intersect(ray)) )
            {

                /** We have a collision **/
                if(!output)
                {   
                    output = temp;
                }
                else
                {
                    if( magnitude(temp->location - temp->source_location) < magnitude(output->location - output->source_location))
                    {
                        output = temp;
                    }
                }
            }
        }
        return output;

    }

    if(this->sub_hierarchies.size() == 0)
    {
        for( auto& triangle : this->triangles)
        {
            if( (temp = triangle.ray_intersect(ray)) )
            {
                if(!output)
                {
                    output = temp;
                }
                else
                {
                    if( magnitude(temp->location - temp->source_location) < magnitude(output->location - output->source_location))
                    {
                        output = temp;
                    }
                }
            }
        }
        return output;

    }

    return output;
}

std::vector<Triangle> Mesh::get_triangles()
{

    return this->triangles;
}


Vector3 Mesh::get_centroid()
{
    Vector3 centroids_sum = { 0.0f, 0.0f, 0.0f};
    for(auto& triangle : this->triangles)
    {

        centroids_sum = centroids_sum + triangle.get_centroid();
    }

    return centroids_sum / this->triangles.size();

}

void Mesh::translate( Vector3 v)
{

    for(auto& triangle : this->triangles)
    {
        triangle.translate(v);
    }

    this->bounding_box = generate_boundingbox(this->triangles);

}

BoundingBox generate_boundingbox(std::vector<Triangle>& triangles)
{
    std::vector<std::shared_ptr<BoundingBox>> boxes;

    for(auto& tri : triangles)
    {
        boxes.push_back( std::make_shared<BoundingBox>( tri.get_boundingbox() ));
    }

    return generate_boundingbox( boxes );
}

void Mesh::subdivide(int current_depth, int sort_axis)
{

    if( current_depth > BOUND_MAX_DEPTH)
        return;

    if( this->triangles.size() < NUM_MODELS_THRESHOLD)
        return;


    /** Functions used to compare centroids of models, for sorting **/
    auto model_compare_x = [](Triangle& a, Triangle& b) { return a.get_centroid().x < b.get_centroid().x; };
    auto model_compare_y = [](Triangle& a, Triangle& b) { return a.get_centroid().y < b.get_centroid().y; };
    auto model_compare_z = [](Triangle& a, Triangle& b) { return a.get_centroid().z < b.get_centroid().z; };


    if( this->triangles.size() < NUM_MODELS_THRESHOLD )
        return;

    if( sort_axis % 3 == 0)
    {
        /** Sort in x axis **/
        std::sort(this->triangles.begin(), this->triangles.end(), model_compare_x);

    }else if( sort_axis % 3 == 1)
    {
        /** Sort in y axis **/
        std::sort(this->triangles.begin(), this->triangles.end(), model_compare_y);

    }else{
        /** Sort in z axis **/
        std::sort(this->triangles.begin(), this->triangles.end(), model_compare_z);
    }


    /** Split vector into 2 **/
    std::size_t const half_size = this->triangles.size() / 2;
    std::vector<Triangle> vec1(this->triangles.begin(), this->triangles.begin() + half_size);
    std::vector<Triangle> vec2(this->triangles.begin() + half_size, this->triangles.end());
    
    Mesh m1( this->color, vec1, current_depth+1, sort_axis+1 );
    Mesh m2( this->color, vec2, current_depth+1, sort_axis+1 );

    this->sub_hierarchies.push_back( m1 );
    this->sub_hierarchies.push_back( m2 );

}

static void parse_model_file( std::ifstream& model_file, ColorRGB color, std::vector<Triangle>& triangle_list)
{

    std::cout << "Processing new model file." << std::endl;

    std::vector<Vector3> vectors;
    std::vector<Face> faces;
    std::vector<Vector3> normals;
    std::vector<Vertex> vertices;

    std::string str;
    while(std::getline(model_file, str))
    {
        std::istringstream iss(str);
        std::string subs;

        iss >> subs;
        
            if( subs == "v")
            {
                iss >> subs;
                float p1 = std::stof(subs);
                iss >> subs;
                float p2 = std::stof(subs);
                iss >> subs;
                float p3 = std::stof(subs);
                vectors.push_back( { p1, p2, p3} );
            }else if( subs == "f" )
            {
                iss >> subs;
                int p1 = std::stoi(subs) - 1;
                iss >> subs;
                int p2 = std::stoi(subs) - 1;
                iss >> subs;
                int p3 = std::stoi(subs) - 1;
                //faces.push_back( {p1, p2, p3});
               faces.push_back( { static_cast<size_t>(p1), static_cast<size_t>(p2), static_cast<size_t>(p3)} );
        }
    }

    for(int i = 0; i < vectors.size(); i++)
    {   
        
        int count = 0;
        std::vector<Vector3> temp_normals;
        for(auto& face : faces)
        {
            if(face.p1 == i || face.p2 == i || face.p3 == i)    /** vertex i is a part of this face **/
            {
                /** Calculate the normal of face i and add it to the list of normals **/
                Vector3 A, B, C;
                A = vectors[ face.p1 ];
                B = vectors[ face.p2 ];
                C = vectors[ face.p3 ];


                Vector3 this_normal = (B - A) * (C - A);
                normalize(this_normal);
                temp_normals.push_back(this_normal);
                count++;

            }
        }

        Vector3 averaged_normal = {0, 0, 0};
        for(auto& normal : temp_normals)
        {
            averaged_normal = averaged_normal + normal;
        }

        averaged_normal = averaged_normal / count;
        normalize(averaged_normal);
        normals.push_back(averaged_normal);

    }

    for(int i = 0; i < vectors.size(); i++)
    {
        vertices.push_back( {vectors[i], normals[i]} );
    }

    for(auto& face: faces)
    {
        triangle_list.push_back( Triangle(color, vertices[face.p1], vertices[face.p2], vertices[face.p3]) );
    }
    
    std::cout << "Finished loading model " << std::endl;

}
