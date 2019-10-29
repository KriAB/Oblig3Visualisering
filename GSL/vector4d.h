#ifndef VECTOR4D_H
#define VECTOR4D_H

#include "gltypes.h"
#include <cmath>
#include <iostream>
#include <cassert>

namespace gsl
{

class Vector3D;
class Matrix4x4;

class Vector4D
{
public:
    //constructors:
    Vector4D(GLfloat x_in=0.f, GLfloat y_in=0.f, GLfloat z_in=0.f, GLfloat w_in = 0.f);
    Vector4D(Vector3D vec3_in, GLfloat w_in);
    Vector4D(const Vector3D &vec3_in);
    Vector4D(const int v);
    Vector4D(const double v);

    // divide x, y, z on w
    void clipInvNormalize();
    void clipNormalize();

    //Operators:
    const Vector4D& operator=(const Vector4D &rhs);     // v = v
    Vector4D operator+(const Vector4D &rhs) const;      // v + v
    Vector4D operator-(const Vector4D &rhs) const;      // v - v
    Vector4D& operator+=(const Vector4D &rhs);          // v += v
    Vector4D& operator-=(const Vector4D &rhs);          // v -= v
    Vector4D operator-() const;                         // -v
    Vector4D operator*(GLfloat rhs) const;              // v * f
    Vector4D operator*(Matrix4x4 q) const;              // v * m
    GLfloat operator* (const Vector4D& v) const;        // Dot product
    Vector4D operator ^ (const Vector4D& v) const;      // Cross product

    GLfloat& operator[] (const int index);

    //functions:
    Vector3D toVector3D();
    Vector3D getVec3(); //OEF: samme som toVector3D()
    void print();
    GLfloat length();
    void normalize();
    Vector4D normalized();
    static GLfloat dot(const Vector4D &v1, const Vector4D &v2);

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);

    //Operator overloading as friendfunction
    friend std::ostream &operator<<(std::ostream &output, const Vector4D &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y <<
                  ", Z = " << rhs.z << ", W = " << rhs.w;
        return output;
    }


    GLfloat getX() const;
    void setX(const GLfloat &value);
    GLfloat getY() const;
    void setY(const GLfloat &value);
    GLfloat getZ() const;
    void setZ(const GLfloat &value);
    GLfloat getW() const;
    void setW(const GLfloat &value);

    Vector3D getXYZ() const;

    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};

} //namespace

#endif // VECTOR4D_H
