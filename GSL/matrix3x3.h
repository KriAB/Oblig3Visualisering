#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "vector3d.h"
#include "gltypes.h"
#include <utility>
#include <iomanip>

namespace gsl
{
    class Matrix2x2;
    class Matrix4x4;
    class Vector3D;

    class Matrix3x3
    {
    public:
        Matrix3x3(bool isIdentity = false);
        Matrix3x3(std::initializer_list<float> values);

        Matrix3x3 identity();
        void setToIdentity();

        GLfloat determinant();

        bool inverse();

        void transpose();

        Matrix2x2 toMatrix2();
        Matrix4x4 toMatrix4();

        Matrix3x3 operator*(const Matrix3x3& other);
        Vector3D operator*(const Vector3D& v);
        GLfloat &operator()(int y, int x);
        GLfloat operator()(int y, int x) const;

        friend std::ostream &operator<<(std::ostream &output, const Matrix3x3 &mIn)
        {
            output << std::setprecision(4) <<
                      "{" << mIn.matrix[0] << "\t, " << mIn.matrix[1] << "\t, " << mIn.matrix[2] << "}\n" <<
                      "{" << mIn.matrix[3] << "\t, " << mIn.matrix[4] << "\t, " << mIn.matrix[5] << "}\n" <<
                      "{" << mIn.matrix[6] << "\t, " << mIn.matrix[7] << "\t, " << mIn.matrix[8] << "}\n";
            return output;
        }

    private:
        GLfloat matrix[9];
    };
}
#endif // MATRIX3X3_H
