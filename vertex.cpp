#include "innpch.h"
#include "vertex.h"

Vertex::Vertex() { }

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    mXYZ.setX(x);
    mXYZ.setY(y);
    mXYZ.setZ(z);

    mNormal.setX(r);
    mNormal.setY(g);
    mNormal.setZ(b);
}

Vertex::Vertex(gsl::Vector3D a, gsl::Vector3D b , gsl::Vector2D c)
{
    mXYZ = a;
    mNormal = b;
    mST = c;
}

Vertex::~Vertex() { }

void Vertex::set_xyz(GLfloat *xyz)
{
    mXYZ.setX(xyz[0]);
    mXYZ.setY(xyz[1]);
    mXYZ.setZ(xyz[2]);
}

void Vertex::set_xyz(GLfloat x, GLfloat y, GLfloat z)
{
    mXYZ.setX(x);
    mXYZ.setY(y);
    mXYZ.setZ(z);
}

void Vertex::set_xyz(gsl::Vector3D xyz_in)
{
    mXYZ = xyz_in;
}

void Vertex::set_rgb(GLfloat *rgb)
{
    mNormal.setX(rgb[0]);
    mNormal.setY(rgb[1]);
    mNormal.setZ(rgb[2]);
}

void Vertex::set_rgb(GLfloat r, GLfloat g, GLfloat b)
{
    mNormal.setX(r);
    mNormal.setY(g);
    mNormal.setZ(b);
}

void Vertex::set_normal(GLfloat *normal)
{
    mNormal.setX(normal[0]);
    mNormal.setY(normal[1]);
    mNormal.setZ(normal[2]);
}

void Vertex::set_normal(GLfloat x, GLfloat y, GLfloat z)
{
    mNormal.setX(x); mNormal.setY(y); mNormal.setZ(z);
}

void Vertex::set_normal(gsl::Vector3D normal_in)
{
    mNormal = normal_in;
}

void Vertex::set_st(GLfloat *st)
{
    mST.setX(st[0]);
    mST.setY(st[1]);
}

void Vertex::set_st(GLfloat s, GLfloat t)
{
    mST.setX(s); mST.setY(t);
}

void Vertex::set_uv(GLfloat u, GLfloat v)
{
    mST.setX(u);
    mST.setY(v);
}

//std::ostream& operator<<(std::ostream& os, const Vertex& v)
//{
//   os << "(" << v.mXYZ.getX() << ", " << v.mXYZ.getY() << ", " << v.mXYZ.getZ() << ") ";
//   os << "(" << v.mNormal.getX() << ", " << v.mNormal.getY() << ", " << v.mNormal.getZ() << ") ";

//   return os;
//}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
    os << std::fixed;
    os << "(" << v.mXYZ.getX() << ", " << v.mXYZ.getY() << ", " << v.mXYZ.getZ() << ") ";
    os << "(" << v.mNormal.getX() << ", " << v.mNormal.getY() << ", " << v.mNormal.getZ() << ") ";
    os << "(" << v.mST.getX() << ", " << v.mST.getY() << ") ";
    return os;
}

std::istream& operator>> (std::istream& is, Vertex& v) {
    // needs 4 temps to get commas and parenthesis
    char temp, temp2, temp3, temp4;
    is >> temp >> v.mXYZ.x >> temp2 >> v.mXYZ.y >> temp3 >> v.mXYZ.z >> temp4;
    is >> temp >> v.mNormal.x >> temp2 >> v.mNormal.y >> temp3 >> v.mNormal.z >> temp4;
    is >> temp >> v.mST.x >> temp2 >> v.mST.y >> temp3;
    return is;
}
