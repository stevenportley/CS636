
#include <optional>
#include "model.h"
#include "triangle.h"
#include "vector3.h"
#include "light.h"


Triangle::Triangle(ColorRGB color, Vertex A, Vertex B, Vertex C) : bounding_box( {},{} )
{
    this->A = A;
    this->B = B;
    this->C = C;
    this->color = color;

    auto max = [](float a, float b, float c)
    {
        if(a > b)
            if(a > c)
                return a;
            else
                return c;
        else
            if(b > c)
                return b;
            else
                return c;
    };

    auto min = [](float a, float b, float c)
    {
        if(a < b)
            if(a < c)
                return a;
            else
                return c;
        else
            if(b < c)
                return b;
            else
                return c;
    };


    Vector3 p1 = { min(this->A.location.x, this->B.location.x, this->C.location.x),
                    min(this->A.location.y, this->B.location.y, this->C.location.y),
                    min(this->A.location.z, this->B.location.z, this->C.location.z) };

    Vector3 p2 = { max(this->A.location.x, this->B.location.x, this->C.location.x),
                    max(this->A.location.y, this->B.location.y, this->C.location.y),
                    max(this->A.location.z, this->B.location.z, this->C.location.z) };

    this->bounding_box = {p1, p2};


}

std::optional<RayCollision> Triangle::ray_intersect( const Ray& ray)
{
    float a_x = this->A.location.x;
    float a_y = this->A.location.y;
    float a_z = this->A.location.z;

    float b_x = this->B.location.x;
    float b_y = this->B.location.y;
    float b_z = this->B.location.z;

    float c_x = this->C.location.x;
    float c_y = this->C.location.y;
    float c_z = this->C.location.z;

    float r_x = ray.origin.x;
    float r_y = ray.origin.y;
    float r_z = ray.origin.z;

    float d_x = ray.direction.x;
    float d_y = ray.direction.y;
    float d_z = ray.direction.z;

    auto determinant = [](float a, float b, float c, float d, float e, float f, float g, float h, float i)
    {
        return a*( (e*i) - f*h) - b*( d*i - f*g) + c*(d*h - e*g);
    };


    float matrix_a = determinant(a_x-b_x, a_x-c_x, d_x, a_y-b_y, a_y-c_y, d_y, a_z-b_z, a_z-c_z, d_z );
    float beta_num = determinant( a_x-r_x, a_x-c_x, d_x, a_y-r_y, a_y-c_y, d_y, a_z-r_z, a_z-c_z, d_z  );
    float gamma_num = determinant( a_x-b_x, a_x-r_x, d_x, a_y-b_y, a_y-r_y, d_y, a_z-b_z, a_z-r_z, d_z );
    float t_num = determinant( a_x-b_x, a_x-c_x, a_x-r_x, a_y-b_y, a_y-c_y, a_y-r_y, a_z-b_z, a_z-c_z, a_z-r_z);

    float beta = beta_num / matrix_a;
    float gamma = gamma_num / matrix_a;
    float alpha = 1.0f - beta - gamma;
    float t = t_num / matrix_a;

    if( beta <= 0)
        return std::optional<RayCollision>();

    if( gamma <= 0)
        return std::optional<RayCollision>();

    if( gamma + beta >= 1)
        return std::optional<RayCollision>();

    if( t <= 0)
        return std::optional<RayCollision>();

    Vector3 point_normal = (alpha * this->A.normal) + (beta * this->B.normal) + (gamma * this->C.normal);
    normalize(point_normal);

    /** We have a collision!! **/
    Vector3 intersection = { 
        ray.origin.x + (t*ray.direction.x),
        ray.origin.y + (t*ray.direction.y),
        ray.origin.z + (t*ray.direction.z),
    };



    RayCollision ray_collision = {
        .color = this->color,
        .location = intersection,
        .normal = point_normal,
        .source_location = ray.origin
    };


    return std::optional<RayCollision>(ray_collision);


}


void Triangle::translate( Vector3 v)
{
    this->A.location = this->A.location + v;
    this->B.location = this->B.location + v;
    this->C.location = this->C.location + v;
}


BoundingBox Triangle::get_boundingbox()
{
    return this->bounding_box;
}

Vector3 Triangle::get_centroid()
{

    return { (this->A.location.x + this->B.location.x + this->C.location.x) / 3,
             (this->A.location.y + this->B.location.y + this->C.location.y) / 3,
             (this->A.location.z + this->B.location.z + this->C.location.z) / 3};

}


