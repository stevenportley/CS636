
#pragma once

#include <optional>
#include "vector3.h"
#include "model.h"
#include "boundingbox.h"

class Sphere: public Model
{
    private:
        Vector3 origin;
        ColorRGB color;
        float radius;
        BoundingBox bounding_box;
    public:
        Sphere(Vector3 origin, float radius, ColorRGB color);
        std::optional<RayCollision> ray_intersect(const Ray& ray, const std::vector<LightSource>& light_sources);
        BoundingBox get_boundingbox();
};

