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

struct Ray
{
    Vector3 origin;
    Vector3 direction;
};

Vector3 operator+( Vector3 const& lhs, Vector3 const& rhs);
Vector3 operator/( Vector3 const& lhs, float const& rhs);
Vector3 operator+( Vector3 const& lhs, float const& rhs);

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