
#pragma once

#include <optional>
#include "model.h"

class Sphere: public Model
{
    private:
        Vector3 origin;
        ColorRGB color;
        float radius;
    public:
        Sphere(Vector3 origin, float radius, ColorRGB color);
        std::optional<RayCollision> ray_intersect(const Ray& ray, const std::vector<LightSource>& light_sources);
};

