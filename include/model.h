#pragma once

#include <vector>
#include <fstream>
#include <iostream>

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Face
{
    ssize_t v1;
    ssize_t v2;
    ssize_t v3;
};


class Model
{
    private:
        std::vector<Face> faces;
        std::vector<Vector3> vertices;
        
    public:
        Model( std::ifstream& model_file );
        void display_contents();

};