#pragma once

#include <vector>
#include "model.h"

class Mesh: public Model
{
    private:
        std::vector<Face> faces;
        std::vector<Vector3> vertices;
        
    public:
        Mesh( std::ifstream& mesh_file );
        Vector3 ray_intersect(const Ray& ray);
        void display_contents();
};
