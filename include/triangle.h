#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"
#include "vector3.h"

class Triangle : public Model
{
    private:
        Vertex A;
        Vertex B;
        Vertex C;
        ColorRGB color;
    public:
        Triangle(ColorRGB color, Vertex A, Vertex B, Vertex C);
        std::optional<RayCollision> ray_intersect( const Ray& ray, const std::vector<LightSource>& light_sources);
};
