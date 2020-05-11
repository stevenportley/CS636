#pragma once



#include <optional>
#include <vector>
#include <memory>
#include "vector3.h"

class BoundingBox 
{

public:
    Vector3 p1;
    Vector3 p2;

public:
    bool does_intersect(const Ray& ray);
    BoundingBox(Vector3 p1, Vector3 p2);

};

#include "model.h"

BoundingBox generate_boundingbox(const std::vector<std::shared_ptr<BoundingBox>>& boxes);
BoundingBox generate_boundingbox(const std::vector<std::shared_ptr<Model>>& models);
