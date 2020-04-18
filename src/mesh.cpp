
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <optional>
#include "vector3.h"
#include "model.h"
#include "mesh.h"
#include "triangle.h"


Mesh::Mesh(std::ifstream& model_file)
{

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
                this->vertices.push_back( {p1, p2, p3} );
            }else if( subs == "f" )
            {
                iss >> subs;
                int p1 = std::stoi(subs) - 1;
                iss >> subs;
                int p2 = std::stoi(subs) - 1;
                iss >> subs;
                int p3 = std::stoi(subs) - 1;
                //faces.push_back( {p1, p2, p3});
               this->faces.push_back( {p1, p2, p3} );
        }
    }
}


void Mesh::display_contents()
{
    for( auto face : faces)
    {
        std::cout << "Face:\n";
        std::cout << vertices[face.p1].x << " " << vertices[face.p1].y << " " << vertices[face.p1].z << std::endl;
        std::cout << vertices[face.p2].x << " " << vertices[face.p2].y << " " << vertices[face.p2].z << std::endl;
        std::cout << vertices[face.p3].x << " " << vertices[face.p3].y << " " << vertices[face.p3].z << std::endl;
        std::cout << std::endl;
    }

}

std::optional<Vector3> Mesh::ray_intersect( const Ray& ray)
{

    std::vector<Vector3> collisions;

    for(auto& face : this->faces)
    {
        Triangle t( this->vertices[face.p1], this->vertices[face.p2], this->vertices[face.p3] );
        
        std::optional<Vector3> tri_intersect = t.ray_intersect(ray);
        if(tri_intersect)
            collisions.push_back(*tri_intersect);

    }

    if(collisions.size() == 0)
        return std::optional<Vector3>();


    if(collisions.size() == 1)
        return std::optional<Vector3>(collisions[0]);


    /** We have collided with mesh more than one time **/
    return std::optional<Vector3>(collisions[0]); //Later figure out which one we have collided with
}

