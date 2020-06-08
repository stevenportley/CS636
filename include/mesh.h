#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"
#include "boundingbox.h"
#include "triangle.h"

struct Face
{
    size_t p1;
    size_t p2;
    size_t p3;
};



class Mesh: public Model
{
    private:
        std::vector<Triangle> triangles;
        std::vector<Mesh> sub_meshes;
        BoundingBox boundingbox;
        ModelProperties model_properties;
        void subdivide(std::vector<Triangle>& triangles, int current_depth, int sort_axis);
        
    public:
        Mesh(ModelProperties model_properties, std::vector<Triangle>& triangles, int current_depth, int sort_axis);
        Mesh( std::ifstream& mesh_file, ModelProperties model_properties);
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect( const Ray& ray);
        void translate( Vector3 v );
};
