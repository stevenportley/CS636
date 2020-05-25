
#include <vector>
#include <memory>
#include <algorithm>
#include <optional>
#include "boundingvolumehierarchy.h"

#define BOUND_MAX_DEPTH 5
#define NUM_MODELS_THRESHOLD 10

BoundingVolumeHierarchy::BoundingVolumeHierarchy( std::vector<std::shared_ptr<Model>>& models, int current_depth, int sort_axis) : bounding_box( {}, {} )
{
    this->model_list = models;
    this->bounding_box = generate_boundingbox(models);
    if(current_depth < BOUND_MAX_DEPTH)
        if(model_list.size() > NUM_MODELS_THRESHOLD)
            this->divide_hierarchies(current_depth, sort_axis);
}



void BoundingVolumeHierarchy::divide_hierarchies(int current_depth, int sort_axis)
{
    /** Functions used to compare centroids of models, for sorting **/
    auto model_compare_x = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().x < b->get_centroid().x; };
    auto model_compare_y = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().y < b->get_centroid().y; };
    auto model_compare_z = [](std::shared_ptr<Model> a, std::shared_ptr<Model> b) { return a->get_centroid().z < b->get_centroid().z; };


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
    std::vector<std::shared_ptr<Model>> vec2(model_list.begin() + half_size, model_list.end());
    
    BoundingVolumeHierarchy b1( vec1, current_depth+1, sort_axis+1 );
    BoundingVolumeHierarchy b2( vec2, current_depth+1, sort_axis+1 );

    this->sub_hierarchies.push_back( b1 );
    this->sub_hierarchies.push_back( b2 );

    return;
}


BoundingBox BoundingVolumeHierarchy::get_boundingbox()
{
    return this->bounding_box;
}

Vector3 BoundingVolumeHierarchy::get_centroid()
{
    return {};
}



std::optional<RayCollision> BoundingVolumeHierarchy::ray_intersect(const Ray& ray)
{

    if( ! this->get_boundingbox().does_intersect(ray) )
        return std::optional<RayCollision>();

    std::optional<RayCollision> closest;
    std::optional<RayCollision> temp;

    if(sub_hierarchies.size() == 0)
    {
        /** We have no subhierarchies (hit limit), so we just check all meshes here **/
        
        for( auto& model : model_list)
        {
            if(temp = model->ray_intersect(ray))
            {
                /** If this model is a collision **/
                if(!closest)
                {
                    closest = temp;
                }else
                {
                    if( magnitude(temp->location - temp->source_location) < magnitude(closest->location - closest->source_location) )
                    {
                        /** If this collision is closer than temp **/
                        closest = temp;

                    }
                }
            }
        }
        return closest;
    }

    for( auto& sub_hierarchy : this->sub_hierarchies)
    {
        if( temp = sub_hierarchy.ray_intersect(ray) )
        {
            if(!closest)
                closest = temp;
            else
                if( magnitude(temp->location - temp->source_location) < magnitude(closest->location - closest->source_location))
                    closest = temp;
        }
    }

    return closest;
}




