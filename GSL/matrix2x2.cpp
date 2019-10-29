#include "matrix2x2.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace gsl
{
    Matrix2x2::Matrix2x2(bool isIdentity)
    {
        if(isIdentity)
        {
            identity();
        }
        else
        {
            for(int i = 0; i < 4; i++)
                matrix[i] = 0.f;
        }
    }

    Matrix2x2::Matrix2x2(std::initializer_list<GLfloat> values)
    {
        int i = 0;
        for(auto value : values)
            matrix[i++] = value;
    }

    Matrix2x2 Matrix2x2::identity()
    {
        setToIdentity();

        return *this;
    }

    void Matrix2x2::setToIdentity()
    {
        *this =
        {
            1, 0,
            0, 1
        };
    }

    GLfloat Matrix2x2::determinant()
    {
        return (matrix[0]*matrix[3] - matrix[1]*matrix[2]);
    }

    bool Matrix2x2::inverse()
    {
        GLfloat det = determinant();

        if(det == 0.f)
            return false;

        det = 1.f / det;

        *this =
        {
             det*matrix[3], -det*matrix[1],
            -det*matrix[2],  det*matrix[0]
        };

        return true;
    }

    void Matrix2x2::transpose()
    {
        std::swap(matrix[1], matrix[2]);
    }

    Matrix3x3 Matrix2x2::toMatrix3()
    {
        return Matrix3x3
        {
            matrix[0], matrix[1], 0,
            matrix[2], matrix[3], 0,
                0,        0,      1
        };
    }

    Matrix4x4 Matrix2x2::toMatrix4()
    {
        return Matrix4x4
        {
            matrix[0], matrix[1], 0, 0,
            matrix[2], matrix[3], 0, 0,
                0,        0,      1, 0,
                0,        0,      0, 1
        };
    }

    Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &other)
    {
        return Matrix2x2
        {
            matrix[0] * other.matrix[0] + matrix[1] * other.matrix[2], matrix[0] * other.matrix[1] + matrix[1] * other.matrix[3],
            matrix[2] * other.matrix[0] + matrix[3] * other.matrix[2], matrix[2] * other.matrix[1] + matrix[3] * other.matrix[3]
        };
    }

    Vector2D Matrix2x2::operator*(const Vector2D &v)
    {
        return Vector2D(matrix[0] * v.x + matrix[1] * v.y, matrix[2] * v.x + matrix[3] * v.y);
    }

    GLfloat &Matrix2x2::operator()(int y, int x)
    {
        return matrix[y * 2 + x];
    }

    GLfloat Matrix2x2::operator()(int y, int x) const
    {
        return matrix[y * 2 + x];
    }
}
