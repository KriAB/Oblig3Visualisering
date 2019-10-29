#include "gsl_math.h"
#include <QDebug>
#include <array>
#include <vector>
#include "matrix4x4.h"
//#include <QElapsedTimer>

namespace gsl
{
    GLdouble rad2deg(GLdouble rad)
    {
        return rad * (180.0 / PI_D);
    }

    GLdouble deg2rad(GLdouble deg)
    {
        return deg * (PI_D / 180.0);
    }

    GLfloat rad2degf(GLfloat rad)
    {
        return rad * (180.0f / PI);
    }

    GLfloat deg2radf(GLfloat deg)
    {
        return deg * (PI / 180.0f);
    }

    GLfloat mod(GLfloat x, GLfloat y)
    {
        // same as (x % y)
        int timesY = static_cast<int>(x / y);
        return x - (timesY * y);
    }

    //Calculates the points on a bezier curve. Input t from 0 to 1
    Vector3D bezierCurve(std::vector<Vector3D> points, GLfloat t, unsigned long long degree)
    {
        for (unsigned long long k = 0; k < degree; ++k) {
            for (unsigned long long i = 0; i < degree - k - 1; ++i)
                points[i] = points[i] * (1.0f - t) + points[i + 1] * t;
        }
        return points[0];
    }

    //Calculates the points on a basis spline curve. Input t from 0 to 1.
    Vector3D bSpline(const std::vector<Vector3D>& points, const std::vector<GLfloat> &t, GLfloat x, unsigned long long degree)
    {
        // CALCULATE VALID KNOT INTERVAL 'MY'
        unsigned long long my;
        for (my = 0; my < points.size(); my++) {
            if (x < t[my+1])
                break;
        }
        // CALCULATE BASIS FUNCTIONS
        std::vector<GLfloat> basis(t.size());
        for (unsigned long long j = 0; j <= degree; j++) {
            for (unsigned long long i = (my-j); i <= my; i++) {
                if (j == 0) {
                    basis[i] = 1.0f;
                } else {
                    GLfloat left, right;
                    if (basis[i] != 0.0f)
                        left = ((x - t[i]) / (t[i+j] - t[i])) * basis[i];
                    else
                        left = 0.0f;
                    if (basis[i+1] != 0.0f)
                        right = ((t[i+j+1] - x) / (t[i+j+1] - t[i+1])) * basis[i+1];
                    else
                        right = 0.0f;
                    basis[i] = left + right;
                }
            }
        }
        // MULTIPLY POINTS WITH BASIS FUNCTIONS
        Vector3D result;
        for (unsigned long long i = 0; i < points.size(); i++)
            result += points[i] * basis[i];
        return result;
    }

    Vector3D up()
    {
        return Vector3D(0.0f, 1.0f, 0.0f);
    }

    Vector3D right()
    {
        return Vector3D(1.0f, 0.0f, 0.0f);
    }

    Vector3D forward()
    {
        return Vector3D(0.0f, 0.0f, 1.0f);
    }

    Vector3D one()
    {
        return Vector3D(1.0f, 1.0f, 1.0f);
    }

    Vector3D zero()
    {
        return Vector3D(0.0f, 0.0f, 0.0f);
    }

    GLfloat clamp(GLfloat x, GLfloat min, GLfloat max)
    {
        if (x < min)
            x = min;
        else if (x > max)
            x = max;
        return x;
    }

    Vector2D lerp2D(GLfloat time, Vector2D a, Vector2D b)
    {
        return a*(1.f-time) + b*time;
    }

    Vector3D lerp3D(GLfloat time, Vector3D a, Vector3D b)
    {
        return a*(1.f-time) + b*time;
    }

    Quaternion qLerp(GLfloat time, Quaternion start, Quaternion end)
    {
        float delta = 1.f-time;

        float x = delta*start[0] + time*end[0];
        float y = delta*start[1] + time*end[1];
        float z = delta*start[2] + time*end[2];
        float w = delta*start[3] + time*end[3];

        Quaternion result(x, y, z, w);
        result.normalize();

        return result;
    }

    Quaternion qLerp(GLfloat time, Vector3D start, Vector3D end)
    {
        Quaternion A(start), B(end);

        Quaternion result((1.f-time)*A[0] + time*B[0],
                          (1.f-time)*A[1] + time*B[1],
                          (1.f-time)*A[2] + time*B[2],
                          (1.f-time)*A[3] + time*B[3]);
        result.normalize();

        return result;
    }

    Vector3D qLerpEuler(GLfloat time, Quaternion start, Quaternion end)
    {
        Quaternion result((1.f-time)*start[0] + time*end[0],
                          (1.f-time)*start[1] + time*end[1],
                          (1.f-time)*start[2] + time*end[2],
                          (1.f-time)*start[3] + time*end[3]);
        result.normalize();

        return result.toEulerAngles();
    }

    Vector3D qLerpEuler(GLfloat time, Vector3D start, Vector3D end)
    {
        Quaternion A(start), B(end);

        Quaternion result((1.f-time)*A[0] + time*B[0],
                          (1.f-time)*A[1] + time*B[1],
                          (1.f-time)*A[2] + time*B[2],
                          (1.f-time)*A[3] + time*B[3]);
        result.normalize();

        return result.toEulerAngles();
    }

    float distanceToPlane(const Vector3D &point, const Vector3D &normal, const Vector3D &pointInPlane)
    {
//        Bruk planformelen Ax + By + Cz - D = 0 (hvor (A,B,C) er plan-normalen og D blir regnet ut ved å legge et punkt i x, y, z).
//        Nærmeste avstand fra et point til dette planet er:
//        d = (Ax+By+Cz-D)/sqrt(A*A + B*B + C*C)
//        hvor x,y,z er koordinatene til point. Hvis resultatet er positivt er punktet på siden av plan-normalen.
//        sqrt(A*A + B*B + C*C) vil være 1, hvis normalen er normalisert!

        float D =
                normal.x * pointInPlane.x +
                normal.y * pointInPlane.y +
                normal.z * pointInPlane.z;

        float distance =
                normal.x * point.x +
                normal.y * point.y +
                normal.z * point.z -
                D;

        return distance;
    }

    bool withinPlane(const Vector3D &point, Matrix4x4 &modelMatrix, Vector2D upright, Vector2D downleft)
    {
        Matrix4x4 inversed = modelMatrix;
        inversed.inverse();
        //rotate point to local space of Plane
        Vector4D transposedPoint = inversed * Vector4D(point, 1.f);

        qDebug() << "TransposedPoint: " << transposedPoint.toVector3D();

        //Test if point is within x and y

        bool xDirection{false};
        bool yDirection{false};
        if (transposedPoint.x <= upright.x && transposedPoint.x >= downleft.x)
            xDirection = true;
        if (transposedPoint.y <= upright.y && transposedPoint.y >= downleft.y)
            yDirection = true;

        if (xDirection && yDirection)
            return true;
        else
            return false;
    }
}
