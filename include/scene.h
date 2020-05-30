#pragma once


class Scene;

#include <optional>
#include <vector>
#include <memory>
#include "model.h"
#include "light.h"

class Scene
{
    private:
        std::vector<Model*> models;
        Vector3 camera_origin;
        Vector3 camera_direction;
        Vector3 camera_view_up_directon;
        float viewplane_distance;
        ColorRGB adap_helper( std::optional<ColorRGB> ss_matrix[5][5], Ray vec_matrix[5][5], int depth, int i, int j, int offset) const;
        ColorRGB adap_ss_ray_trace(Vector3 image_plane_origin, Vector3 horiz_diff, Vector3 horiz_diff_next, Vector3 vert_diff, Vector3 vert_diff_next) const;

    public:
        Scene(float viewplane_distance,
                Vector3 camera_direction,
                Vector3 camera_view_up_direction,
                Vector3 camera_origin);
        std::optional<RayCollision> ray_trace(const Ray& ray) const;
        std::vector<LightSource> light_sources;
        void add_model(Model* new_model);
        void add_light(LightSource& new_light);
        void render() const;
        
};
