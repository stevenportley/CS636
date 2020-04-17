#pragma once

#include <optional>
#include <vector>
#include "model.h"
#include "vector3.h"

class Triangle : public Model
{
    private:
        Vector3 a;
        Vector3 b;
        Vector3 c;
    public:
        Triangle(Vector3 p1, Vector3 p2, Vector3 p3);
        std::optional<Vector3> ray_intersect( const Ray& ray);
};
