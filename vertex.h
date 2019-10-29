#ifndef VERTEX_H
#define VERTEX_H

#include "vector2d.h"
#include "vector3d.h"

class Vertex {
public:
    Vertex();
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(gsl::Vector3D a, gsl::Vector3D b, gsl::Vector2D c);
    ~Vertex();

    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);

    void set_xyz(GLfloat *xyz);
    void set_xyz(GLfloat x, GLfloat y, GLfloat z);
    void set_xyz(gsl::Vector3D xyz_in);
    void set_rgb(GLfloat *rgb);
    void set_rgb(GLfloat r, GLfloat g, GLfloat b);
    void set_normal(GLfloat *normal);
    void set_normal(GLfloat x, GLfloat y, GLfloat z);
    void set_normal(gsl::Vector3D normal_in);
    void set_st(GLfloat *st);
    void set_st(GLfloat s, GLfloat t);
    void set_uv(GLfloat u, GLfloat v);
    gsl::Vector3D get_XYZ(){return mXYZ;}

private:
    gsl::Vector3D mXYZ;
    gsl::Vector3D mNormal;
    gsl::Vector2D mST;
};

#endif // VERTEX_H
