#include <iostream>
#include <cmath>
#include <limits>
#include <optional>
#include "vector3.h"
#include "model.h"
#include "light.h"
#include "scene.h"

#define MAX_RECUR_DEPTH 3

ColorRGB calculate_light(const RayCollision& intersection_data, const Scene& scene, int recursion_depth/*=0*/, bool is_inside_model/*=false*/)
{
    if(recursion_depth >= MAX_RECUR_DEPTH)
        return intersection_data.model_properties.color;


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
        
        total_intensity = total_intensity + (light.light * intersection_data.model_properties.kd * n_dot_l);


        /** Specular reflection **/
        float h_dot_n = dot_product(H, intersection_data.normal);
        if( h_dot_n < 0.0f )
            h_dot_n = 0.0f;
        else
            h_dot_n = std::pow(h_dot_n, SHININESS);

        total_intensity = total_intensity + (light.light * intersection_data.model_properties.ks * h_dot_n); 

    }
   
    /** Ambient light **/
    total_intensity = total_intensity + (ia * intersection_data.model_properties.ka);


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
        ColorRGB reflection_color = calculate_light( *reflection_collision, scene, recursion_depth+1, is_inside_model);
        total_intensity = total_intensity + (intersection_data.model_properties.kr * reflection_color * (*reflection_collision).model_properties.color);
    }



    /** Refraction light **/
    auto refract_ray = [=](const RayCollision& ray_collision, bool is_outside_model, Vector3 view_direction)
    {
        float n_dot_v = dot_product(ray_collision.normal, view_direction);
        float n_r = is_outside_model ? (1.0f/ray_collision.model_properties.refr_index) : ray_collision.model_properties.refr_index;
        float radicand = (1.0f - ( (n_r*n_r) * (1.0f - ( n_dot_v * n_dot_v))));

        Vector3 refraction_direction;
        if(radicand < 0)
            refraction_direction = -1.0 * view_direction;
        else
            refraction_direction = (((n_r * n_dot_v) - std::sqrt(radicand)) * ray_collision.normal) - (n_r*view_direction);

        normalize(refraction_direction);
        
        Vector3 refraction_location = ray_collision.location + (ray_collision.normal * -0.0001);

        Ray this_ray {
            .origin = refraction_location,
            .direction = refraction_direction
        };

        return this_ray;
    };

    Ray refraction_ray = refract_ray(intersection_data, true, V);

    std::optional<RayCollision> refraction_collision = scene.ray_trace(refraction_ray);
    if(refraction_collision)
    {
        Vector3 view_direction = refraction_collision->source_location - refraction_collision->location;
        normalize(view_direction);
        Ray new_refraction_ray = refract_ray(*refraction_collision, false, view_direction);
        
        std::optional<RayCollision> new_refraction_collision = scene.ray_trace(new_refraction_ray);
        if(new_refraction_collision)
        {
            ColorRGB refraction_color = calculate_light(*new_refraction_collision, scene, recursion_depth+1, ~is_inside_model);
            total_intensity = total_intensity + (intersection_data.model_properties.kt * refraction_color);
        }
    }

    ColorRGB color = intersection_data.model_properties.color * total_intensity;

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


