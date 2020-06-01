#include <iostream>
#include <cmath>
#include <limits>
#include <optional>
#include "vector3.h"
#include "model.h"
#include "light.h"
#include "scene.h"

#define MAX_REFL_DEPTH 5


ColorRGB ks = { 0.3f, 0.3f, 0.3f};
ColorRGB kd = { 0.7f, 0.7f, 0.7f};
ColorRGB kr = { 0.6f, 0.6f, 0.6f};
ColorRGB ka = { 0.4f, 0.4f, 0.4f};

ColorRGB calculate_light(const RayCollision& intersection_data, const Scene& scene, int recursion_depth/* =0 */)
{
    if(recursion_depth >= MAX_REFL_DEPTH)
        return intersection_data.color;

    Vector3 V = intersection_data.source_location - intersection_data.location;
    normalize(V);
  
    ColorRGB ia = {0.4f, 0.4f, 0.4f };

    ColorRGB total_intensity = {0.0f, 0.0f, 0.0f};

    for( auto& light : scene.light_sources)
    {
        
        /** Common parameters **/
        Vector3 L = light.location - intersection_data.location;
        normalize(L);
        Vector3 H = (L+V) / magnitude(L+V);
        normalize(H);

        /** Shadow check **/
        
        // Move shadow ray origin ever so slightly in the direction of the light source to avoid collisions with ourself **/
        Vector3 shadow_location = intersection_data.location + ( intersection_data.normal * 0.0001f) ;

        Ray shadow_ray{
            .origin = shadow_location,
            .direction = L
        };

        std::optional<RayCollision> shadow_collision = scene.ray_trace( shadow_ray );

        if(shadow_collision)
        {
            float light_distance = magnitude(light.location - intersection_data.location);
            float model_distance = magnitude(shadow_collision->location - intersection_data.location);
            /** Is collision block the light source? **/
            if( model_distance < light_distance)
                continue;
        }

        
        /** Diffuse reflection **/
        float n_dot_l = dot_product(intersection_data.normal, L);
        if( n_dot_l < 0)
            n_dot_l = 0.0f;
        
        total_intensity = total_intensity + (light.light * kd * n_dot_l);


        /** Specular reflection **/
        float h_dot_n = dot_product(H, intersection_data.normal);
        if( h_dot_n < 0.0f )
            h_dot_n = 0.0f;
        else
            h_dot_n = std::pow(h_dot_n, SHININESS);

        total_intensity = total_intensity + (light.light * ks * h_dot_n); 

    }
   
    /** Ambient light **/
    total_intensity = total_intensity + (ia * ka);


    /** Reflection light **/

    Vector3 reflection_direction = ((2.0f * dot_product(intersection_data.normal, V)) * intersection_data.normal) - V;
    normalize(reflection_direction);
    Vector3 reflection_origin = intersection_data.location + (0.0001 * intersection_data.normal);

    Ray reflection_ray {
        .origin = reflection_origin,
        .direction = reflection_direction
    };

    std::optional<RayCollision> reflection_collision = scene.ray_trace( reflection_ray );

    if( reflection_collision)
    {
        ColorRGB reflection_color = calculate_light( *reflection_collision, scene, recursion_depth+1);
        total_intensity = total_intensity + (kr * reflection_color * (*reflection_collision).color);
    }

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

std::ostream& operator<<(std::ostream& os, ColorRGB const& rhs)
{
    os << "[" << rhs.red << " " << rhs.green << " " << rhs.blue << "]";
    return os;
}


