
#include <iostream>
#include <cmath>

#include "vector3.h"


Vector3 operator+( Vector3 const& lhs, Vector3 const& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vector3 operator+( Vector3 const& lhs, float const& rhs)
{
    return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};

}

Vector3 operator/( Vector3 const& lhs, float const& rhs)
{
    return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};

}
Vector3 operator-(Vector3 const& lhs, float const& rhs)
{
    return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
}

Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Vector3 operator*( Vector3 const& lhs, Vector3 const& rhs)
{
    Vector3 out;
    out.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
    out.y = (lhs.x * rhs.z) - (lhs.z * rhs.x);
    out.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
    return out;
}

Vector3 operator*( Vector3 const& lhs, float const& rhs)
{
    return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

Vector3 operator*(float const& lhs, Vector3 const& rhs)
{
    return { rhs.x * lhs, rhs.y * lhs, rhs.z * lhs};
}

float dot_product(Vector3 const& lhs, Vector3 const& rhs)
{
    return ( (lhs.x*rhs.x) + (lhs.y*rhs.y) + (lhs.z*rhs.z) );
}

std::ostream& operator<<(std::ostream& os, Vector3 const& rhs)
{
    os << "[" << rhs.x << " " << rhs.y << " " << rhs.z << "]";
    return os;
}


void normalize(Vector3& vector)
{
    float magnitude = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
    vector.x = vector.x / magnitude;
    vector.y = vector.y / magnitude;
    vector.z = vector.z / magnitude;
}

float magnitude(const Vector3& vector)
{
    return sqrt( (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z) );
}
