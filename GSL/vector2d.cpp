#include "vector2d.h"
#include "vector3d.h"
#include <cmath>

namespace gsl
{
Vector2D::Vector2D(GLfloat x_in, GLfloat y_in) : x{x_in}, y{y_in} { }

Vector2D::Vector2D(const int v):
    x{ static_cast<float>(v) }, y{ static_cast<float>(v) }
{ }

Vector2D::Vector2D(const double v) :
    x{ static_cast<float>(v) }, y{ static_cast<float>(v) }
{ }

const Vector2D &Vector2D::operator =(const Vector2D &rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D &rhs) const
{
    return { x + rhs.x, y + rhs.y };
}

Vector2D Vector2D::operator-(const Vector2D &rhs) const
{
    return { x - rhs.x, y - rhs.y };
}

Vector2D &Vector2D::operator+=(const Vector2D &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2D Vector2D::operator-() const
{
    return { -x, -y };
}

Vector2D Vector2D::operator*(float lhs) const
{
    return { x * lhs, y * lhs };
}

GLfloat Vector2D::length() const
{
    return sqrt(x*x + y*y);
}

//friend function
Vector2D operator*(const Vector2D &lhs, const Vector2D &rhs)
{
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

GLfloat Vector2D::getX() const
{
    return x;
}

void Vector2D::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vector2D::getY() const
{
    return y;
}

void Vector2D::setY(const GLfloat &value)
{
    y = value;
}

void Vector2D::normalize()
{
    GLfloat l = length();
    if (l > 0.f)
    {
        x = x / l;
        y = y / l;
    }
}

Vector2D Vector2D::normalized()
{
    Vector2D normalized;
    GLfloat l = length();

    if (l > 0.f)
    {
        normalized.setX(x / l);
        normalized.setY(y / l);
    }

    return normalized;
}

GLfloat Vector2D::cross2(const Vector2D &v1, const Vector2D &v2)
{
    return (v1.getX() * v2.getY()) - (v1.getY() * v2.getX());
}

Vector3D Vector2D::barycentricCoordinates(const Vector2D &p1, const Vector2D &p2, const Vector2D &p3)
{
    Vector2D p12 = p2-p1; //x1
    Vector2D p13 = p3-p1; //x2

    Vector3D n = Vector3D(0,0,cross2(p12,p13)); //arealet mellom p12 og p13
    GLfloat areal_123 = n.z; //n.length();//dobbelt areal

    Vector3D baryc; //til retur. husk
    //u
    Vector2D p = p2 - *this; //u1
    Vector2D q = p3 - *this;//u2
    n = Vector3D(0,0, cross2(p,q));
    baryc.x = n.z/areal_123;

    //v
    p = p3- *this; //v1
    q = p1 - *this; //v2
    n = Vector3D(0,0, cross2(p,q));
    baryc.y = n.z/areal_123;

    //w
    p = p1 - *this; //w1
    q = p2 -*this; //w2
    n = Vector3D(0,0, cross2(p,q));
    baryc.z = n.z/areal_123;
   // std::cout << " baryryry: " << baryc << std::endl;
    return baryc;
}

void Vector2D::print()
{
    std::cout << "(" << x << "," << y << ")\n";
}

//friend function
std::ostream &operator<<(std::ostream &out, const Vector2D &rhs)
{
    out << "(" << rhs.x << "," << rhs.y << ")";
    return out;
}

float cross(const Vector2D &v1, const Vector2D &v2)
{
    return abs((v1.x * v2.y) - (v1.y * v2.x));
}

float dot(const Vector2D  &v1, const Vector2D  &v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

Vector2D normalize(const Vector2D &v)
{
    float l = sqrt(v.x * v.x + v.y * v.y);
    return { v.x / l, v.y / l };
}

} //namespace
