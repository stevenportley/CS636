
#pragma once

#include "vector3.h"
#include "model.h"

#define KD 0.6f     /** Diffuse surface reflectance coefficient **/
#define KS 0.5f     /** Specular surface reflectance coefficient **/
#define KA 0.3f     /** Ambient surface reflectance coefficeint **/
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

ColorRGB calculate_light(RayCollision& intersection_data, const std::vector<LightSource>& light_sources, const std::vector<Model*>& models);
ColorRGB operator+(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, float const& rhs);
ColorRGB operator*(float const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator/(ColorRGB const& lhs, int const& rhs);
ColorRGB operator-(ColorRGB const& lhs, ColorRGB const& rhs);
