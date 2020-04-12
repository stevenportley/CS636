#pragma once

#include <vector>
#include "model.h"

struct ViewPlane
{
    Vector3 origin;
    ssize_t width;
    ssize_t height;
};

class Scene
{
    private:
        std::vector<Model> models;
        Vector3 camera_origin;
        ViewPlane view_plane;

    public:
        Scene(ssize_t view_plane_height, 
                ssize_t view_plane_width, 
                int distance,
                Vector3 view_direction, 
                Vector3 camera_origin);
        
};