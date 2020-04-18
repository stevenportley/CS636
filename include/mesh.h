#pragma once

#include <optional>
#include <vector>
#include "model.h"

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
        std::vector<Vector3> vertices;
        
    public:
        Mesh( std::ifstream& mesh_file );
        std::optional<Vector3> ray_intersect(const Ray& ray);
        void display_contents();
};
