#include "vector4d.h"
#include "vector3d.h"
#include <cmath>

namespace gsl
{

Vector4D::Vector4D(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat w_in)
    : x{x_in}, y{y_in}, z{z_in}, w{w_in}
{ }

Vector4D::Vector4D(Vector3D vec3_in, GLfloat w_in)
    : x{vec3_in.x}, y{vec3_in.getY()}, z{vec3_in.getZ()}, w{w_in}
{ }

Vector4D::Vector4D(const Vector3D &vec3_in)
    : x{vec3_in.x}, y{vec3_in.getY()}, z{vec3_in.getZ()}, w{1.0f}
{ }

Vector4D::Vector4D(const int v)
    : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{1.0f}
{ }

Vector4D::Vector4D(const double v)
    : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{1.0f}
{ }

void Vector4D::clipInvNormalize()
{
    x /= w;
    y /= w;
    z /= w;
    w = 1;
}
void Vector4D::clipNormalize()
{
    w = 1/w;
    x *= w;
    y *= w;
    z *= w;
}

GLfloat Vector4D::operator *(const Vector4D &v) const
{
    return (x*v.x + y*v.y + z*v.z + w*v.w);
}

GLfloat &Vector4D::operator[](const int index)
{
    assert(index <4 && index >=0);

    switch (index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    }
    return x;   //to silence compiler warnings
}

const Vector4D& Vector4D::operator=(const Vector4D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
}
Vector4D Vector4D::operator+(const Vector4D& rhs) const
{
    return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
}

Vector4D Vector4D::operator-(const Vector4D& rhs) const
{
    return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
}

Vector4D& Vector4D::operator+=(const Vector4D &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}
Vector4D& Vector4D::operator-=(const Vector4D &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}
Vector4D Vector4D::operator-() const
{
    return {-x, -y, -z, -w};
}

Vector4D Vector4D::operator*(GLfloat rhs) const
{
    return {x * rhs, y * rhs, z * rhs, w * rhs};
}

GLfloat Vector4D::length()
{
    return std::sqrt(x*x + y*y + z*z + w*w);
}


Vector3D Vector4D::toVector3D()
{
    return Vector3D(x, y, z);
}

Vector3D Vector4D::getVec3()
{
    return { x, y, z };
}

void Vector4D::normalize()
{
    GLfloat l = length();
    if(l > 0.f)
    {
        x = x / l;
        y = y / l;
        z = z / l;
        w = w / l;
    }
}
Vector4D Vector4D::normalized()
{
    Vector4D normalized;
    GLfloat l = length();
    if (l > 0.f)
    {
        normalized.setX(x / l);
        normalized.setY(y / l);
        normalized.setZ(z / l);
        normalized.setW(w / l);
    }
    return normalized;
}

GLfloat Vector4D::dot(const Vector4D &v1, const Vector4D &v2)
{
    return (v1.x * v2.x) + (v1.getY() * v2.getY()) + (v1.getZ() * v2.getZ()) + (v1.getW() * v2.getW());
}

void Vector4D::print()
{
    std::cout << "(" << x << "," << y << "," << z << "," << w << ")\n";
}

void Vector4D::rotateX(GLfloat angle)
{
    Vector3D v = toVector3D();
    v.rotateX(angle);
    x = v.x;
    y = v.y;
    z = v.z;
}
void Vector4D::rotateY(GLfloat angle)
{
    Vector3D v = toVector3D();
    v.rotateY(angle);
    x = v.x;
    y = v.y;
    z = v.z;
}
void Vector4D::rotateZ(GLfloat angle)
{
    Vector3D v = toVector3D();
    v.rotateZ(angle);
    x = v.x;
    y = v.y;
    z = v.z;
}

GLfloat Vector4D::getX() const
{
    return x;
}

void Vector4D::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vector4D::getY() const
{
    return y;
}

void Vector4D::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vector4D::getZ() const
{
    return z;
}

void Vector4D::setZ(const GLfloat &value)
{
    z = value;
}

GLfloat Vector4D::getW() const
{
    return w;
}
void Vector4D::setW(const GLfloat &value)
{
    if(value == 0.f || value == 1.f)    //w should be only 0 or 1
        w = value;
}

Vector3D Vector4D::getXYZ() const
{
    return Vector3D(x, y, z);
}

} //namespace
