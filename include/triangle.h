#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"
#include "vector3.h"
#include "boundingbox.h"

class Triangle : public Model
{
    private:
        Vertex A;
        Vertex B;
        Vertex C;
        ColorRGB color;
    public:
        Triangle(ColorRGB color, Vertex A, Vertex B, Vertex C);
        BoundingBox generate_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect( const Ray& ray, const std::vector<LightSource>& light_sources);
};
