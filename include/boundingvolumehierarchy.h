#pragma once


#include <vector>
#include <memory>
#include "model.h"
#include "boundingbox.h"



class BoundingVolumeHierarchy : public Model
{
    private:
        std::vector<std::shared_ptr<Model>> model_list;
        std::vector<std::shared_ptr<BoundingBox>> bounding_boxes;
        std::vector<BoundingVolumeHierarchy> sub_hierarchies;
        void divide_hierarchies(int current_depth, int sort_axis);

    public:
        BoundingVolumeHierarchy( std::vector<std::shared_ptr<Model>>& models, int current_depth, int sort_axis);
        BoudingBox generate_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect(const Ray& ray, const std::vector<LightSource>& light_sources);
        void display_contents();

};

