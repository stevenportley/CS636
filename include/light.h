
#pragma once

#include "vector3.h"


#define KD 0.7f     /** Diffuse surface reflectance coefficient **/
#define KS 0.5f     /** Specular surface reflectance coefficient **/
#define KA 0.4f     /** Ambient surface reflectance coefficeint **/
#define IA 0.25f
#define SHININESS 30


struct ColorRGB{
    float red;
    float green;
    float blue;
};

struct LightSource{
    Vector3 location;
    ColorRGB light;
};


#include "model.h"
ColorRGB calculate_light(ColorRGB model_color, Vector3 view_origin, Vector3 intersection, Vector3 normal, const std::vector<LightSource>& light_sources);
ColorRGB operator+(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, float const& rhs);
ColorRGB operator*(float const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator/(ColorRGB const& lhs, int const& rhs);
ColorRGB operator-(ColorRGB const& lhs, ColorRGB const& rhs);
