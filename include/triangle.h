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
        BoundingBox bounding_box;
    public:
        Triangle(ColorRGB color, Vertex A, Vertex B, Vertex C);
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        void translate(Vector3 v);
        std::optional<RayCollision> ray_intersect( const Ray& ray);
};
