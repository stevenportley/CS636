
#include <cmath>
#include "vector3.h"
#include "model.h"
#include "light.h"


ColorRGB calculate_light(RayCollision& intersection_data, const std::vector<LightSource>& light_sources, const std::vector<Model*>& models)
{
    ColorRGB ia = {0.4, 0.4, 0.4};


    ColorRGB total_intensity = {0.0f, 0.0f, 0.0f};
    for( auto& light : light_sources)
    {
        bool is_shadow = false;
        
        Vector3 shadow_direction = light.location - intersection_data.location;
        normalize(shadow_direction);
        
        /** Move shadow ray origin ever so slightly in the direction of the shadow ray to avoid recollisions **/
        Vector3 shadow_location = intersection_data.location + (0.01 * intersection_data.normal);

        Ray shadow_ray{
            .origin = shadow_location,
            .direction = shadow_direction,
        };

        for( auto& model : models)
        {
            if( model->ray_intersect( shadow_ray) )
            {
                is_shadow = true;
                break;
            }
        }

        if(is_shadow)
            continue;

        Vector3 object_to_light = light.location - intersection_data.location;
        normalize(object_to_light);
        normalize(intersection_data.normal);
        float n_dot_l = dot_product(intersection_data.normal, object_to_light);
        if( n_dot_l < 0)
            n_dot_l = 0.0f;

        total_intensity = total_intensity + (light.light * KD * n_dot_l); /** Diffuse reflection **/

        Vector3 reflected_ray = (2 * n_dot_l * intersection_data.normal) - object_to_light;

        normalize(reflected_ray);
        Vector3 v = intersection_data.source_location - intersection_data.location;
        normalize(v);
        float r_dot_v = dot_product(reflected_ray, v);
        r_dot_v = std::pow(r_dot_v, SHININESS);
        if( r_dot_v < 0.0f)
            r_dot_v = 0.0f;


        total_intensity = total_intensity + (light.light * KS * r_dot_v); /** Specular reflection **/


    }

    total_intensity = total_intensity + (ia * KA);

    ColorRGB color = intersection_data.color * total_intensity;

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

ColorRGB operator/(ColorRGB const& lhs, int const& rhs)
{
    return{
        .red = lhs.red / rhs,
        .green = lhs.green / rhs,
        .blue = lhs.blue / rhs,
    };
}

ColorRGB operator-(ColorRGB const& lhs, ColorRGB const& rhs)
{
    return{
        .red = lhs.red - rhs.red,
        .green = lhs.green - rhs.green,
        .blue = lhs.blue - rhs.blue
    };
};



