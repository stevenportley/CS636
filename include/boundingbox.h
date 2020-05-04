#pragma once

#include <optional>
#include <vector>
#include "vector3.h"
#include "model.h"

class BoundingBox 
{

public:
    Vector3 p1;
    Vector3 p2;

public:
    bool does_intersect(const Ray& ray);
    BoundingBox(Vector3 p1, Vector3 p2);
    BoundingBox(const std::vector<BoundingBox>& boxes);

};