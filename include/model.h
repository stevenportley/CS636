#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "vector3.h"

struct Ray
{
    Vector3 origin;
    Vector3 direction;
};

struct Face
{
    ssize_t v1;
    ssize_t v2;
    ssize_t v3;
};

class Model{
    public:
        virtual Vector3 ray_intersect( const Ray& ray) = 0;
};
