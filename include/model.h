#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include "vector3.h"



class Model{
    public:
        virtual std::optional<Vector3> ray_intersect( const Ray& ray) = 0;
};
