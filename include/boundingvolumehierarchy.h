#pragma once


#include <vector>
#include <memory>
#include "model.h"
#include "boundingbox.h"


class BoundingVolumeHierarchy : public Model
{
    private:
        std::vector<std::shared_ptr<Model>> model_list;
        std::vector<BoundingVolumeHierarchy> sub_hierarchies;
        BoundingBox bounding_box;
        void divide_hierarchies(int current_depth, int sort_axis);
    public:
        BoundingVolumeHierarchy( std::vector<std::shared_ptr<Model>>& models, int current_depth, int sort_axis);
        BoundingBox get_boundingbox();
        Vector3 get_centroid();
        std::optional<RayCollision> ray_intersect(const Ray& ray);

};

