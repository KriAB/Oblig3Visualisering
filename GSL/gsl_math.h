#ifndef GSL_MATH_H
#define GSL_MATH_H

#include <vector>
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "quaternion.h"
#include "matrix3x3.h"
#include "math_constants.h"

namespace gsl
{
    class Quaternion;

    // Functions
    GLdouble rad2deg(GLdouble rad);
    GLdouble deg2rad(GLdouble deg);
    GLfloat rad2degf(GLfloat rad);
    GLfloat deg2radf(GLfloat deg);
    GLfloat mod(GLfloat x, GLfloat y);
    GLfloat clamp(GLfloat x, GLfloat min, GLfloat max);

    // Interpolation
    // remember time can only be between 0 and 1
    Vector2D lerp2D(GLfloat time, Vector2D start, Vector2D end); // Move a straight line with linear time-step between points
    Vector3D lerp3D(GLfloat time, Vector3D start, Vector3D end);

    // Lerp between quaternion rotations,
    // remember time can only be between 0 and 1
    Quaternion qLerp(GLfloat time, Quaternion start, Quaternion end);
    Quaternion qLerp(GLfloat time, Vector3D start, Vector3D end);
    Vector3D qLerpEuler(GLfloat time, Quaternion start, Quaternion end);
    Vector3D qLerpEuler(GLfloat time, Vector3D start, Vector3D end);

    // Curves
    Vector3D bezierCurve(std::vector<Vector3D> points, GLfloat t, unsigned long long degree = 3);
    Vector3D bSpline(const std::vector<Vector3D>& points, const std::vector<GLfloat>& t, GLfloat x, unsigned long long degree = 3);

    // Basic vector directions
    Vector3D up();
    Vector3D right();
    Vector3D forward();
    Vector3D one();
    Vector3D zero();

    // Distance and Collision tests
    float distanceToPlane(const Vector3D &point, const Vector3D &normal, const Vector3D &pointInPlane);
    bool withinPlane(const Vector3D &point, Matrix4x4 &modelMatrix, Vector2D upright, Vector2D downleft);
}

#endif // GSL_MATH_H
