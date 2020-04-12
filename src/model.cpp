
#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(std::ifstream& model_file)
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


void Model::display_contents()
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

Vector3 operator+( Vector3 const& lhs, Vector3 const& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vector3 operator/( Vector3 const& lhs, float const& rhs)
{
    return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};

}

Vector3 operator+( Vector3 const& lhs, float const& rhs)
{
    return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};

}