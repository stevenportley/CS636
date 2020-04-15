#include "sphere.h"

Sphere::Sphere(Vector3 origin, float radius)
{
    this->origin = origin;
    this->radius = radius;
}

std::optional<Vector3> Sphere::ray_intersect(const Ray& ray)
{


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
                (r_s * r_s);;

    float discriminant = (B * B) - (4 * C);

    if(discriminant < 0)
        return std::optional<Vector3>();

    float t_o = ((-1 * B) - discriminant) / 2;

    Vector3 output;
    output.x = x_o + (x_d * t_o);
    output.y = y_o + (y_d * t_o);
    output.z = z_o + (z_d * t_o);

    return std::optional<Vector3>( output );


}

