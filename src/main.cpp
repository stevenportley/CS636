
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include "face.h"
#include "vector3.h"


int main(int argc, char** argv)
{

    if( argc != 2)
    {
        std::cout << "Usage: main <file>\n";
        return -1;
    }

    std::ifstream in(argv[1]);
    if(!in)
    {
        std::cout << "Reading file failed!\n";
        return -1;
    }

    std::vector<CS636::Vector3> vertices;
    std::vector<CS636::Face> faces;
        

    std::string str;
    while(std::getline(in, str))
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
                vertices.push_back( CS636::Vector3(p1, p2, p3) );
            }else if( subs == "f" )
            {
                iss >> subs;
                int p1 = std::stoi(subs);
                iss >> subs;
                int p2 = std::stoi(subs);
                iss >> subs;
                int p3 = std::stoi(subs);
                faces.push_back( CS636::Face(vertices[p1], vertices[p2], vertices[p3]) );
        }
    }

    for(auto f : faces)
    {
        auto v = f.v1;
        std::cout << v.x << " " << v.y << " " << v.z << std::endl;
        v = f.v2;
        std::cout << v.x << " " << v.y << " " << v.z << std::endl;
        v = f.v3;
        std::cout << v.x << " " << v.y << " " << v.z << std::endl;
        std::cout << std::endl;
    }

    
    return 0;

}
