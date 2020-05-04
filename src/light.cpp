
#include <cmath>
#include "vector3.h"
#include "model.h"
#include "light.h"


ColorRGB calculate_light(ColorRGB model_color, Vector3 view_origin, Vector3 intersection, Vector3 normal, const std::vector<LightSource>& light_sources)
{

    ColorRGB ia = {0.25, 0.25, 0.25};

    ColorRGB total_intensity = {0.0f, 0.0f, 0.0f};
    for( auto& light : light_sources)
    {
        Vector3 object_to_light = light.location - intersection;
        normalize(object_to_light);
        float n_dot_l = dot_product(normal, object_to_light);
        if( n_dot_l < 0)
            n_dot_l = 0.0f;

        total_intensity = total_intensity + (light.light * KD * n_dot_l); /** Diffuse reflection **/

        Vector3 reflected_ray = (2 * n_dot_l * normal) - object_to_light;
        normalize(reflected_ray);
        Vector3 v = view_origin - intersection;
        normalize(v);
        float r_dot_v = dot_product(reflected_ray, v);
        r_dot_v = std::pow(r_dot_v, SHININESS);
        if( r_dot_v < 0.0f)
            r_dot_v = 0.0f;

        total_intensity = total_intensity + (light.light * KS * r_dot_v); /** Specular reflection **/


    }

    total_intensity = total_intensity + (ia * KA);

    ColorRGB color = model_color * total_intensity;

    if(color.red > 1.0f)
        color.red = 1.0f;

    if(color.green > 1.0f)
        color.green = 1.0f;

    if(color.blue > 1.0f)
        color.blue = 1.0f;

    return color;

}



ColorRGB operator+(ColorRGB const& lhs, ColorRGB const& rhs)
{
    return{
        .red = lhs.red + rhs.red,
        .green = lhs.green + rhs.green,
        .blue = lhs.blue + rhs.blue
    };
}

ColorRGB operator*(ColorRGB const& lhs, float const& rhs)
{
    return{
        .red = lhs.red * rhs,
        .green = lhs.green * rhs,
        .blue = lhs.blue * rhs
    };
}


ColorRGB operator*(float const& lhs, ColorRGB const& rhs)
{
    return{
        .red = rhs.red * lhs,
        .green = rhs.green * lhs,
        .blue = rhs.blue * lhs
    };
}


ColorRGB operator*(ColorRGB const& lhs, ColorRGB const& rhs)
{
    return{
        .red = rhs.red * lhs.red,
        .green = rhs.green * lhs.green,
        .blue = rhs.blue * lhs.blue
    };
}