
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


Mesh::Mesh(std::ifstream& model_file, ColorRGB color) : bounding_box({}, {})
{

    std::cout << "Processing new model file." << std::endl;

    std::vector<Vector3> vectors;
    std::vector<int> vertex_count; /** Indicies for vectors/normals/count all match **/

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
               this->faces.push_back( { static_cast<size_t>(p1), static_cast<size_t>(p2), static_cast<size_t>(p3)} );
        }
    }

    for(int i = 0; i < vectors.size(); i++)
    {   
        
        int count = 0;
        std::vector<Vector3> temp_normals;
        for(auto& face : this->faces)
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
        this->normals.push_back(averaged_normal);

    }

    for(int i = 0; i < vectors.size(); i++)
    {
        this->vertices.push_back( {vectors[i], this->normals[i]} );
    }

    this->color = color;


    Vector3 min = vertices[0].location;
    Vector3 max = vertices[0].location;

    for(auto& vertex : vertices)
    {
        if(vertex.location.x < min.x)
            min.x = vertex.location.x;

        if(vertex.location.x > max.x)
            max.x = vertex.location.x;

        if(vertex.location.y < min.y)
            min.y = vertex.location.y;

        if(vertex.location.y > max.y)
            max.y = vertex.location.y;

        if(vertex.location.z < min.z)
            min.z = vertex.location.z;

        if(vertex.location.z > max.z)
            max.z = vertex.location.z;

    }



    BoundingBox box(min, max);

    this->bounding_box = box;


    std::cout << "Finished loading model " << std::endl;
    std::cout << "Bounding box: [" << min.x << min.y << min.z << "] [" << max.x << max.y << max.z << "]" << std::endl;

}


void Mesh::display_contents()
{
    /**
    for( auto face : faces)
    {
        std::cout << "Face:\n";
        std::cout << vertices[face.p1].x << " " << vertices[face.p1].y << " " << vertices[face.p1].z << std::endl;
        std::cout << vertices[face.p2].x << " " << vertices[face.p2].y << " " << vertices[face.p2].z << std::endl;
        std::cout << vertices[face.p3].x << " " << vertices[face.p3].y << " " << vertices[face.p3].z << std::endl;
        std::cout << std::endl;
    }
    **/

}

std::optional<RayCollision> Mesh::ray_intersect( const Ray& ray, const std::vector<LightSource>& light_sources)
{

    /** Miss bounding box, early drop out **/
    if(!(this->bounding_box.does_intersect(ray)))
    {
        return std::optional<RayCollision>();
    }




    std::vector<RayCollision> collisions;
    for(auto& face : this->faces)
    {
        Triangle t( this->color, this->vertices[face.p1], this->vertices[face.p2], this->vertices[face.p3]);
        
        std::optional<RayCollision> tri_intersect = t.ray_intersect(ray, light_sources);
        if(tri_intersect)
            collisions.push_back(*tri_intersect);

    }

    if(collisions.size() == 0)
        return std::optional<RayCollision>();


    if(collisions.size() == 1)
        return std::optional<RayCollision>(collisions[0]);


    /** We have collided with mesh more than one time **/
    RayCollision temp = collisions[0];
    for(auto& collision : collisions)
    {
        if( magnitude(collision.location - ray.origin) < magnitude(temp.location - ray.origin)  )
            temp = collision;

    }

    return std::optional<RayCollision>(temp); 
}



BoundingBox Mesh::generate_boundingbox()
{

    Vector3 min = this->vertices[0].location;
    Vector3 max = this->vertices[0].location;

    for(auto& vertex : this->vertices)
    {
        if(vertex.location.x < min.x)
            min.x = vertex.location.x;

        if(vertex.location.x > max.x)
            max.x = vertex.location.x;

        if(vertex.location.y < min.y)
            min.y = vertex.location.y;

        if(vertex.location.y > max.y)
            max.y = vertex.location.y;

        if(vertex.location.z < min.z)
            min.z = vertex.location.z;

        if(vertex.location.z > max.z)
            max.z = vertex.location.z;

    }

    BoundingBox box(min, max);

    return box;

}

Vector3 Mesh::get_centroid()
{
    Vector3 centroids_sum = { 0.0f, 0.0f, 0.0f};
    for(auto& vertex : this->vertices)
    {
        centroids_sum = centroids_sum + vertex.location;
    }

    return centroids_sum / this->vertices.size();

}



