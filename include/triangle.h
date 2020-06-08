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
        ModelProperties model_properties;
        BoundingBox bounding_box;
        void generate_boundingbox();
    public:
        Triangle(Vertex A, Vertex B, Vertex C, ModelProperties model_properties);
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        void translate(Vector3 v);
        std::optional<RayCollision> ray_intersect( const Ray& ray);
};


