#pragma once

#include <vector>
#include "model.h"

class Scene
{
    private:
        std::vector<Model> models;
        Vector3 camera_origin;
        Vector3 camera_direction;
        int viewplane_distance;

    public:
        Scene(int viewplane_distance,
                Vector3 camera_direction,
                Vector3 camera_origin);
        void add_model(Model& new_model);
        void render();
        
};