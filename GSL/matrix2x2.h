#ifndef MATRIX2X2_H
#define MATRIX2X2_H

#include "vector2d.h"
#include "vector3d.h"
#include "gltypes.h"
#include <utility>
#include <iomanip>

namespace gsl
{
    class Matrix3x3;
    class Matrix4x4;

    class Matrix2x2
    {
    public:
        Matrix2x2(bool isIdentity = false);
        Matrix2x2(std::initializer_list<GLfloat> values);

        Matrix2x2 identity();
        void setToIdentity();

        GLfloat determinant();
        bool inverse();

        void transpose();

        Matrix3x3 toMatrix3();
        Matrix4x4 toMatrix4();

        Matrix2x2 operator*(const Matrix2x2 &other);
        Vector2D operator*(const Vector2D &v);
        GLfloat& operator()(int y, int x);
        GLfloat operator()(int y, int x) const;

        friend std::ostream& operator<<(std::ostream &output, const Matrix2x2 &mIn)
        {
            output << std::setprecision(4) <<
                      "{" << mIn.matrix[0] << "\t, " << mIn.matrix[1] << "}\n" <<
                      "{" << mIn.matrix[2] << "\t, " << mIn.matrix[3] << "}\n";
            return output;
        }
    private:
        GLfloat matrix[4];
    };
}
#endif // MATRIX2X2_H
