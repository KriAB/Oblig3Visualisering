#include "matrix3x3.h"
#include "matrix2x2.h"
#include "matrix4x4.h"

namespace gsl
{
    Matrix3x3::Matrix3x3(bool isIdentity)
    {
        if(isIdentity)
        {
            identity();
        }
        else
        {
            for(int i = 0; i < 9; i++)
                matrix[i] = 0.f;
        }
    }

    Matrix3x3::Matrix3x3(std::initializer_list<float> values)
    {
        int i = 0;
        for(auto value : values)
            matrix[i++] = value;
    }

    Matrix3x3 Matrix3x3::identity()
    {
        setToIdentity();

        return *this;
    }

    void Matrix3x3::setToIdentity()
    {
        *this =
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
    }

    GLfloat Matrix3x3::determinant()
    {
        //det = a(ei − fh) − b(di − fg) + c(dh − eg)
        // where
        //    a, b, c,
        //    d, e, f
        //    g, h, i

        return matrix[0]*(matrix[4]*matrix[8]-matrix[5]*matrix[7])
             - matrix[1]*(matrix[3]*matrix[8]-matrix[5]*matrix[6])
             + matrix[2]*(matrix[3]*matrix[7]-matrix[4]*matrix[6]);
    }

    bool Matrix3x3::inverse()
    {
        GLfloat det = determinant();

        if(det == 0.f)
            return false;

        GLfloat a = (*this)(0, 0), b = (*this)(0, 1), c = (*this)(0, 2),
                d = (*this)(1, 0), e = (*this)(1, 1), f = (*this)(1, 2),
                g = (*this)(2, 0), h = (*this)(2, 1), i = (*this)(2, 2);

        GLfloat A =  (e*i-f*h), B = -(d*i-f*g), C =  (d*h-e*g),
                D = -(b*i-c*h), E =  (a*i-c*g), F = -(a*h-b*g),
                G =  (b*f-c*e), H = -(a*f-c*d), I =  (a*e-b*d);

        det = 1.f/det;

        *this =
        {
            det*A, det*D, det*G,
            det*B, det*E, det*H,
            det*C, det*F, det*I
        };

        return true;
    }

    void Matrix3x3::transpose()
    {
        std::swap(matrix[1], matrix[4]);
        std::swap(matrix[2], matrix[6]);
        std::swap(matrix[5], matrix[7]);
    }

    Matrix2x2 Matrix3x3::toMatrix2()
    {
        return Matrix2x2
        {
            matrix[0], matrix[1],
            matrix[3], matrix[4]
        };
    }

    Matrix4x4 Matrix3x3::toMatrix4()
    {
        return Matrix4x4
        {
            matrix[0], matrix[1], matrix[2], 0,
            matrix[3], matrix[4], matrix[5], 0,
            matrix[6], matrix[7], matrix[8], 0,
                0,        0,         0,      0
        };
    }

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other)
    {
        return
        {
            matrix[0] * other.matrix[0] + matrix[1] * other.matrix[3] + matrix[2] * other.matrix[6],
            matrix[0] * other.matrix[1] + matrix[1] * other.matrix[4] + matrix[2] * other.matrix[7],
            matrix[0] * other.matrix[2] + matrix[1] * other.matrix[5] + matrix[2] * other.matrix[8],

            matrix[3] * other.matrix[0] + matrix[4] * other.matrix[3] + matrix[5] * other.matrix[6],
            matrix[3] * other.matrix[1] + matrix[4] * other.matrix[4] + matrix[5] * other.matrix[7],
            matrix[3] * other.matrix[2] + matrix[4] * other.matrix[5] + matrix[5] * other.matrix[8],

            matrix[6] * other.matrix[0] + matrix[7] * other.matrix[3] + matrix[8] * other.matrix[6],
            matrix[6] * other.matrix[1] + matrix[7] * other.matrix[4] + matrix[8] * other.matrix[7],
            matrix[6] * other.matrix[2] + matrix[7] * other.matrix[5] + matrix[8] * other.matrix[8]
        };
    }

    Vector3D Matrix3x3::operator*(const Vector3D &v)
    {
        return Vector3D{
            matrix[0] * v.x + matrix[1] * v.y + matrix[2] * v.z,
            matrix[3] * v.x + matrix[4] * v.y + matrix[5] * v.z,
            matrix[6] * v.x + matrix[7] * v.y + matrix[8] * v.z
        };
    }

    GLfloat &Matrix3x3::operator()(int y, int x)
    {
        return matrix[y * 3 + x];
    }

    GLfloat Matrix3x3::operator()(int y, int x) const
    {
        return matrix[y * 3 + x];
    }
}
