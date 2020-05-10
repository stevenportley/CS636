
#include <vector>
#include <memory>
#include <algorithm>
#include <optional>
#include "model.h"
#include "boundingbox.h"
#include "boundingvolumehierarchy.h"

#define BOUND_MAX_DEPTH 3
#define NUM_MODELS_THRESHOLD 10

BoundingVolumeHierarchy::BoundingVolumeHierarchy( std::vector<std::shared_ptr<Model>>& models, int current_depth, int sort_axis)
{
    for(auto& model : models)
    {
        this->model_list.push_back(model);
        temp.push_back(*model);
    }

    this->boundingbox = generate_boundingbox(temp);


    this->divide_hierarchies(current_depth, sort_axis);
}



void BoundingVolumeHierarchy::divide_hierarchies(int current_depth, int sort_axis)
{
    /** Functions used to compare centroids of models, for sorting **/
    auto model_compare_x = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().x < b->get_centroid().x; };
    auto model_compare_y = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().y < b->get_centroid().y; };
    auto model_compare_z = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().z < b->get_centroid().z; };

    std::vector<std::shared_ptr<BoundingBox>> bounding_box_list;
    for(auto& model : model_list)
    {
        bounding_box_list.push_back(std::make_shared<BoundingBox>(model->get_boundingbox());
    }

    BoundingBox bounding_box = generate_boundingbox(bounding_box_list);

    if(model_list.size() < NUM_MODELS_THRESHOLD)
        return;

    if( sort_axis % 3 == 0)
    {
        /** Sort in x axis **/
        std::sort(model_list.begin(), model_list.end(), model_compare_x);

    }else if( sort_axis % 3 == 1)
    {
        /** Sort in y axis **/
        std::sort(model_list.begin(), model_list.end(), model_compare_y);

    }else{
        /** Sort in z axis **/
        std::sort(model_list.begin(), model_list.end(), model_compare_z);
    }


    /** Split vector into 2 **/
    std::size_t const half_size = this->model_list.size() / 2;
    std::vector<std::shared_ptr<Model>> vec1(model_list.begin(), model_list.begin() + half_size);
    std::vector<std::shared_ptr<Model>> vec2(model_list.begin() + half_size, model_list.begin());
    
    BoundingVolumeHierarchy b1( vec1, current_depth+1, sort_axis+1 );
    BoundingVolumeHierarchy b2( vec2, current_depth+1, sort_axis+1 );

    this->sub_hierarchies.push_back( b1 );
    this->sub_hierarchies.push_back( b2 );

    return;
}


BoundingBox BoundingVolumeHierarchy::get_boundingbox()
{

    return this->boundingbox;
}

Vector3 BoundingVolumeHierarchy::get_centroid()
{
    return {};
}



std::optional<RayCollision> BoundingVolumeHierarchy::ray_intersect(const Ray& ray, const std::vector<LightSource>& light_sources)
{
    return std::optional<RayCollision>();

}




