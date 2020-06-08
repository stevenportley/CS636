
#include <iostream>
#include <algorithm>
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
#define BOUND_MAX_DEPTH 5

static BoundingBox generate_boundingbox(std::vector<Mesh>& sub_meshes);
static BoundingBox generate_boundingbox(std::vector<Triangle>& triangles);
static void parse_model_file( std::ifstream& model_file, ModelProperties model_properties, std::vector<Triangle>& triangle_list);

Mesh::Mesh(ModelProperties model_properties, std::vector<Triangle>& triangles, int current_depth, int sort_axis) : boundingbox({}, {})
{
    if(triangles.size() == 0)
    {
        std::cout << "Creating mesh with 0 triangles" << std::endl;
        return;
    }
    this->subdivide(triangles, current_depth, sort_axis);
}


Mesh::Mesh(std::ifstream& model_file, ModelProperties model_properties) : boundingbox({}, {})
{
    this->model_properties = model_properties;
    std::vector<Triangle> triangle_list;
    parse_model_file(model_file, model_properties, triangle_list);
    this->subdivide(triangle_list, 0, 0);
    std::cout << "Boundingbox: " << this->boundingbox << std::endl;
}

BoundingBox Mesh::get_boundingbox()
{
    return this->boundingbox;
}

std::optional<RayCollision> Mesh::ray_intersect( const Ray& ray)
{
    
    std::optional<RayCollision> output;
    std::optional<RayCollision> temp;

    /** Miss bounding box, early drop out **/
    if(!(this->boundingbox.does_intersect(ray)))
    {   
        return std::optional<RayCollision>();
    }

    if(this->sub_meshes.size() == 2)
    {
        for(auto& sub_mesh : this->sub_meshes)
        {
            if( (temp = sub_mesh.ray_intersect(ray)) )
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


    if(this->sub_meshes.size() == 0)
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

    std::cout << "Should never be here" << std::endl;

    return output;
}


Vector3 Mesh::get_centroid()
{
    std::cout << "get_centroid not implemented" << std::endl;
    exit(1);
    Vector3 centroids_sum = { 0.0f, 0.0f, 0.0f};
    for(auto& triangle : this->triangles)
    {
        centroids_sum = centroids_sum + triangle.get_centroid();
    }

    return centroids_sum / this->triangles.size();

}

void Mesh::translate( Vector3 v)
{

    if( this->sub_meshes.size() > 0)
    {
        for(auto& sub_mesh : sub_meshes)
        {
            sub_mesh.translate(v);
        }
        this->boundingbox = generate_boundingbox(this->sub_meshes);
    }else
    {
        for(auto& triangle : this->triangles)
        {
            triangle.translate(v);
        }
        this->boundingbox = generate_boundingbox(this->triangles);
    }

}


void Mesh::subdivide(std::vector<Triangle>& triangles, int current_depth, int sort_axis)
{

    if( (current_depth > BOUND_MAX_DEPTH) || (triangles.size() < NUM_MODELS_THRESHOLD))
    {
        this->triangles = triangles;
        this->boundingbox = generate_boundingbox(this->triangles);
        return;
    }

    /** Functions used to compare centroids of models, for sorting **/
    auto model_compare_x = [](Triangle& a, Triangle& b) { return a.get_centroid().x < b.get_centroid().x; };
    auto model_compare_y = [](Triangle& a, Triangle& b) { return a.get_centroid().y < b.get_centroid().y; };
    auto model_compare_z = [](Triangle& a, Triangle& b) { return a.get_centroid().z < b.get_centroid().z; };

    if( sort_axis % 3 == 0)
    {
        /** Sort in x axis **/
        std::sort(triangles.begin(), triangles.end(), model_compare_x);

    }else if( sort_axis % 3 == 1)
    {
        /** Sort in y axis **/
        std::sort(triangles.begin(), triangles.end(), model_compare_y);

    }else{
        /** Sort in z axis **/
        std::sort(triangles.begin(), triangles.end(), model_compare_z);
    }

    /** Split vector into 2 **/
    std::size_t const half_size = triangles.size() / 2;
    std::vector<Triangle> vec1(triangles.begin(), triangles.begin() + half_size);
    std::vector<Triangle> vec2(triangles.begin() + half_size, triangles.end());

    Mesh m1( this->model_properties, vec1, current_depth+1, sort_axis+1 );
    Mesh m2( this->model_properties, vec2, current_depth+1, sort_axis+1 );


    this->sub_meshes.push_back( m1 );
    this->sub_meshes.push_back( m2 );

    this->boundingbox = generate_boundingbox(this->sub_meshes);

}

static void parse_model_file( std::ifstream& model_file, ModelProperties model_properties, std::vector<Triangle>& triangle_list)
{

    std::cout << "Processing new model file." << std::endl;

    std::vector<Vector3> vert_locations;
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
                vert_locations.push_back( { p1, p2, p3} );
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

    for(int i = 0; i < vert_locations.size(); i++)
    {   
        
        std::vector<Vector3> temp_normals;
        for(auto& face : faces)
        {
            if(face.p1 == i || face.p2 == i || face.p3 == i)    /** vertex i is a part of this face **/
            {
                /** Calculate the normal of face i and add it to the list of normals **/
                Vector3 A, B, C;
                A = vert_locations[ face.p1 ];
                B = vert_locations[ face.p2 ];
                C = vert_locations[ face.p3 ];


                Vector3 this_normal = (B - A) * (C - A);
                normalize(this_normal);
                temp_normals.push_back(this_normal);


            }
        }

        Vector3 averaged_normal = {0, 0, 0};
        for(auto& normal : temp_normals)
        {
            averaged_normal = averaged_normal + normal;
        }

        //averaged_normal = averaged_normal / temp_normals.size();
        normalize(averaged_normal);
        normals.push_back(averaged_normal);

    }

    for(int i = 0; i < vert_locations.size(); i++)
    {
        vertices.push_back( {vert_locations[i], normals[i]} );
    }

    for(auto& face: faces)
    {
        triangle_list.push_back( Triangle(vertices[face.p1], vertices[face.p2], vertices[face.p3], model_properties));
    }
    
    std::cout << "Finished loading model " << std::endl;

}



static BoundingBox generate_boundingbox(std::vector<Triangle>& triangles)
{
    std::vector<BoundingBox> boxes;
    for(auto& triangle : triangles)
    {
        boxes.push_back(triangle.get_boundingbox());
    }

    return generate_boundingbox(boxes);
}

static BoundingBox generate_boundingbox(std::vector<Mesh>& sub_meshes)
{
    std::vector<BoundingBox> boxes;
    for(auto& sub_mesh : sub_meshes)
    {
        boxes.push_back( sub_mesh.get_boundingbox() );
    }

    return generate_boundingbox(boxes);

}
