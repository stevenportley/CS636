#include <cmath>
#include "vector3.h"
#include "model.h"
#include "sphere.h"

Sphere::Sphere(Vector3 origin, float radius, ColorRGB color) : bounding_box( {}, {})
{
    this->origin = origin;
    this->radius = radius;
    this->color = color;

    Vector3 p1 = { this->origin.x - this->radius,
                    this->origin.y - this->radius,
                    this->origin.z - this->radius };

    Vector3 p2 = { this->origin.x + this->radius,
                    this->origin.y + this->radius,
                    this->origin.z + this->radius };

   this->bounding_box = { p1, p2}; 
}



std::optional<RayCollision> Sphere::ray_intersect(const Ray& ray, const std::vector<LightSource>& light_sources)
{
    if( !this->bounding_box.does_intersect(ray) )
        return std::optional<RayCollision>();
         
    // d is ray
    // c is sphere
    float x_d = ray.direction.x;
    float y_d = ray.direction.y;
    float z_d = ray.direction.z;

    float x_o = ray.origin.x;
    float y_o = ray.origin.y;
    float z_o = ray.origin.z;

    float x_c = this->origin.x;
    float y_c = this->origin.y;
    float z_c = this->origin.z;

    float r_s = this->radius;

    float B = 2 * (  (x_d * (x_o - x_c)) + 
                     (y_d * (y_o - y_c)) + 
                     (z_d * (z_o - z_c)) );

    float C =   ((x_o - x_c) * (x_o - x_c)) +
                ((y_o - y_c) * (y_o - y_c)) + 
                ((z_o - z_c) * (z_o - z_c)) - 
                (r_s * r_s);

    float discriminant = (B * B) - (4 * C);

    if(discriminant < 0)
        return std::optional<RayCollision>();

    float sqrt_discriminant = sqrt(discriminant);

    float t_0 = ((-1 * B) - sqrt_discriminant) / 2;
    float t_1 = ((-1 * B) + sqrt_discriminant) / 2;

    Vector3 intersection;
    Vector3 surface_normal;
    if(t_0 > 0) 
    {
        /** t_0 is first point hit **/
        intersection.x = x_o + (x_d * t_0);
        intersection.y = y_o + (y_d * t_0);
        intersection.z = z_o + (z_d * t_0);
        surface_normal = intersection - this->origin;
        normalize(surface_normal);
    }else if(t_1 > 0)
    {
        /** t_1 is first point hit, we are inside sphere **/
        intersection.x = x_o + (x_d * t_1);
        intersection.y = y_o + (y_d * t_1);
        intersection.z = z_o + (z_d * t_1);
        surface_normal = intersection - this->origin;
        surface_normal = surface_normal * -1;
        normalize(surface_normal);
    }else{
        /** Sphere is behind us **/
        return std::optional<RayCollision>();
    }

    ColorRGB color = calculate_light(this->color, ray.origin, intersection, surface_normal, light_sources);

    //Vector3 surface_normal = 
    RayCollision output =
    {
        .color = color,
        .location = intersection,
        .normal = surface_normal,
        .source_location = ray.origin
    };

    return std::optional<RayCollision>(output);

}



BoundingBox Sphere::get_boundingbox()
{
    return this->bounding_box;
}









