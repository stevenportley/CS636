
#pragma once

#include <optional>
#include "vector3.h"
#include "model.h"
#include "boundingbox.h"

class Sphere: public Model
{
    private:
        Vector3 origin;
        ModelProperties model_properties;
        float radius;
        BoundingBox bounding_box;
    public:
        Sphere(Vector3 origin, float radius, ModelProperties model_properties);
        std::optional<RayCollision> ray_intersect(const Ray& ray);
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
};

