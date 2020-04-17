
#include <optional>
#include "model.h"
#include "triangle.h"



Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3)
{
    this->a = p1;
    this->b = p2;
    this->c = p3;
}

std::optional<Vector3> Triangle::ray_intersect( const Ray& ray)
{
    float a_x = this->a.x;
    float a_y = this->a.y;
    float a_z = this->a.z;

    float b_x = this->b.x;
    float b_y = this->b.y;
    float b_z = this->b.z;

    float c_x = this->c.x;
    float c_y = this->c.y;
    float c_z = this->c.z;

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

/**
    float matrix_a = (((a_x - b_x) * (a_y - c_y) * d_z) - ((a_x - b_x) * d_y * (a_z - c_z))) -
                    (((a_x - c_x) * d_y * (a_z - b_z)) - ((a_x - c_x) * (a_y - b_y) * d_z)) +
                    ((d_x * (a_y - b_y) * (a_z - c_z)) - ( d_x * (a_y - c_y) * (a_z - b_z)));
**/

    float matrix_a = determinant(a_x-b_x, a_x-c_x, d_x, a_y-b_y, a_y-c_y, d_y, a_z-b_z, a_z-c_z, d_z );
    float beta_num = determinant( a_x-r_x, a_x-c_x, d_x, a_y-r_y, a_y-c_y, d_y, a_z-r_z, a_z-c_z, d_z  );
    float gamma_num = determinant( a_x-b_x, a_x-r_x, d_x, a_y-b_y, a_y-r_y, d_y, a_z-b_z, a_z-r_z, d_z );
    float t_num = determinant( a_x-b_x, a_x-c_x, a_x-r_x, a_y-b_y, a_y-c_y, a_y-r_y, a_z-b_z, a_z-c_z, a_z-r_z);

    float beta = beta_num / matrix_a;
    float gamma = gamma_num / matrix_a;
    float t = t_num / matrix_a;

    if( beta <= 0)
        return std::optional<Vector3>();

    if( gamma <= 0)
        return std::optional<Vector3>();

    if( gamma + beta >= 1)
        return std::optional<Vector3>();

    if( t <= 0)
        return std::optional<Vector3>();

    Vector3 intersection = { 
        ray.origin.x + t*ray.direction.x,
        ray.origin.y + t*ray.direction.y,
        ray.origin.z + t*ray.direction.z,
    };

    return std::optional<Vector3>(intersection);


}