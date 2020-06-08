
#pragma once

struct ColorRGB;
struct LightSource;

#include "vector3.h"
#include "model.h"
#include "scene.h"

#define KD 0.7f     /** Diffuse surface reflectance coefficient **/
#define KS 0.5f     /** Specular surface reflectance coefficient **/
#define KA 0.5f     /** Ambient surface reflectance coefficeint **/
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

ColorRGB calculate_light(const RayCollision& intersection_data, const Scene& scene, int recursion_depth=0, bool is_inside_model=false);
std::ostream& operator<<(std::ostream& os, ColorRGB const& rhs);
ColorRGB operator+(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, float const& rhs);
ColorRGB operator*(float const& lhs, ColorRGB const& rhs);
ColorRGB operator*(ColorRGB const& lhs, ColorRGB const& rhs);
ColorRGB operator/(ColorRGB const& lhs, int const& rhs);
ColorRGB operator-(ColorRGB const& lhs, ColorRGB const& rhs);
