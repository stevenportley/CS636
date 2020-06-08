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

struct ModelProperties
{
    ColorRGB color;
    float refr_index;
    ColorRGB ks;
    ColorRGB kd;
    ColorRGB kr;
    ColorRGB kt;
    ColorRGB ka;
};

struct RayCollision
{
    Vector3 location;
    Vector3 normal;
    Vector3 source_location;
    ModelProperties model_properties;
};

class Model{
    public:
        virtual std::optional<RayCollision> ray_intersect( const Ray& ray) = 0;
        virtual BoundingBox get_boundingbox() = 0;
        virtual Vector3 get_centroid() = 0;
};
