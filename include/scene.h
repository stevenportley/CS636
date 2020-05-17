#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "model.h"

class Scene
{
    private:
        std::vector<Model*> models;
        std::vector<LightSource> light_sources;
        Vector3 camera_origin;
        Vector3 camera_direction;
        Vector3 camera_view_up_directon;
        float viewplane_distance;
        ColorRGB ray_trace(const Ray& ray);
        ColorRGB adap_helper( std::optional<ColorRGB> ss_matrix[5][5], Ray vec_matrix[5][5], int depth, int i, int j, int offset);
        ColorRGB adap_ss_ray_trace(Vector3 image_plane_origin, Vector3 horiz_diff, Vector3 horiz_diff_next, Vector3 vert_diff, Vector3 vert_diff_next);

    public:
        Scene(float viewplane_distance,
                Vector3 camera_direction,
                Vector3 camera_view_up_direction,
                Vector3 camera_origin);
        void add_model(Model* new_model);
        void add_light(LightSource& new_light);
        void render();
        
};
