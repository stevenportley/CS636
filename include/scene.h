#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"

#define VRES 256
#define HRES 256


class Scene
{
    private:
        std::vector<Model*> models;
        std::vector<LightSource> light_sources;
        Vector3 camera_origin;
        Vector3 camera_direction;
        Vector3 camera_view_up_directon;
        float viewplane_distance;
        std::optional<ColorRGB> ray_trace(const Ray& ray);


    public:
        Scene(float viewplane_distance,
                Vector3 camera_direction,
                Vector3 camera_view_up_direction,
                Vector3 camera_origin);
        void add_model(Model* new_model);
        void add_light(LightSource& new_light);
        void render();
        
};