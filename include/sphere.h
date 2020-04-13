
#pragma once

#include "model.h"

class Sphere: public Model
{
    private:
        Vector3 origin;
        float radius;
    public:
        Sphere(Vector3 origin, float radius);
        Vector3 ray_intersect(const Ray& ray);
};

