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
        BoundingBox bounding_box;
        ColorRGB color;
        
    public:
        Mesh( std::ifstream& mesh_file, ColorRGB color );
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect( const Ray& ray);
        std::vector<Triangle> get_triangles();
        void translate( Vector3 v );
        void display_contents();
};
