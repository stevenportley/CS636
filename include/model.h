#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include "vector3.h"
#include "boundingbox.h"

struct Vertex{
    Vector3 location;
    Vector3 normal;
};

#include "light.h"
struct RayCollision
{
    ColorRGB color;
    Vector3 location;
    Vector3 normal;
    Vector3 source_location;
};

class Model{
    private:
        BoundingBox bounding_box;
    public:
        virtual std::optional<RayCollision> ray_intersect( const Ray& ray, const std::vector<LightSource>& light_sources) = 0;
        virtual BoundingBox get_boundingbox() = 0;
        virtual Vector3 get_centroid() = 0;
};
