#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"

class Scene
{
    private:
        std::vector<std::unique_ptr<Model>> models;
        Vector3 camera_origin;
        Vector3 camera_direction;
        int viewplane_distance;
        std::optional<Vector3> ray_trace(const Ray& ray);

    public:
        Scene(int viewplane_distance,
                Vector3 camera_direction,
                Vector3 camera_origin);
        void add_model(std::unique_ptr<Model> new_model);
        void render();
        
};