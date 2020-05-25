#pragma once

class Model;
struct RayCollision;

#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include "vector3.h"
#include "boundingbox.h"
#include "light.h"

struct Vertex{
    Vector3 location;
    Vector3 normal;
};

struct RayCollision
{
    ColorRGB color;
    Vector3 location;
    Vector3 normal;
    Vector3 source_location;
};

class Model{
    public:
        virtual std::optional<RayCollision> ray_intersect( const Ray& ray) = 0;
        virtual BoundingBox get_boundingbox() = 0;
        virtual Vector3 get_centroid() = 0;
};
