
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <optional>
#include "vector3.h"
#include "model.h"
#include "mesh.h"

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
                vertices.push_back( {p1, p2, p3} );
            }else if( subs == "f" )
            {
                iss >> subs;
                int p1 = std::stoi(subs);
                iss >> subs;
                int p2 = std::stoi(subs);
                iss >> subs;
                int p3 = std::stoi(subs);
                //faces.push_back( {p1, p2, p3});
                faces.push_back( { p1, p2, p3 });
        }
    }
}


void Mesh::display_contents()
{
    for( auto face : faces)
    {
        std::cout << "Face:\n";
        std::cout << vertices[face.v1].x << " " << vertices[face.v1].y << " " << vertices[face.v1].z << std::endl;
        std::cout << vertices[face.v2].x << " " << vertices[face.v2].y << " " << vertices[face.v2].z << std::endl;
        std::cout << vertices[face.v3].x << " " << vertices[face.v3].y << " " << vertices[face.v3].z << std::endl;
        std::cout << std::endl;
    }

}

std::optional<Vector3> Mesh::ray_intersect( const Ray& ray)
{
    return std::optional<Vector3>();
}

