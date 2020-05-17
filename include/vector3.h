#pragma once

#include <iostream>

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Ray
{
    Vector3 origin;
    Vector3 direction;
};


Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);
Vector3 operator+(Vector3 const& lhs, float const& rhs);
Vector3 operator-(Vector3 const& lhs, float const& rhs);
Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs);
Vector3 operator/(Vector3 const& lhs, float const& rhs);
Vector3 operator*(Vector3 const& lhs, Vector3 const& rhs); /** Vector cross-product **/
Vector3 operator*(Vector3 const& lhs, float const& rhs);
Vector3 operator*(float const& lhs, Vector3 const& rhs);
std::ostream& operator<<(std::ostream& os, Vector3 const& rhs);

float dot_product(const Vector3& lhs, const Vector3& rhs);
void normalize(Vector3& vector);
float magnitude(const Vector3& vector);
