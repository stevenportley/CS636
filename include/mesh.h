#pragma once

#include <optional>
#include <vector>
#include "model.h"
#include "boundingbox.h"

struct Face
{
    size_t p1;
    size_t p2;
    size_t p3;
};



class Mesh: public Model
{
    private:
        std::vector<Face> faces; /** a face is defined as 3 indicies into the vertices vector **/
        std::vector<Vertex> vertices;
        BoundingBox bounding_box;
        ColorRGB color;
        /** Put in a ref count later, shared ptr? **/
        std::vector<Vector3> normals;
        
    public:
        Mesh( std::ifstream& mesh_file, ColorRGB color );
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect( const Ray& ray, const std::vector<LightSource>& light_sources);
        void display_contents();
};
