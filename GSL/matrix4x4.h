#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <iostream>
#include <iomanip>
#include "gltypes.h"
#include "vector3d.h"
#include "vector4d.h"
#include "gsl_math.h"

namespace gsl
{
    class Matrix2x2;
    class Matrix3x3;

    //This matrix class was based on Lars M. Nylands code

    /// This class uses Column Major layout for the internal data
    /// Translate is in the last column - index 3, 7, 11
    class Matrix4x4
    {
    public:
        Matrix4x4(bool isIdentity = false);
        Matrix4x4(std::initializer_list<GLfloat> values);

        // Operators
        GLfloat operator()(const unsigned int& y, const unsigned int& x);
        GLfloat operator()(const unsigned int& y, const unsigned int& x) const;

        Matrix4x4 operator*(const Matrix4x4 &other);
        Vector4D operator*(const Vector4D &other);

        GLfloat getFloat(int space);
        GLfloat operator[](const unsigned int index);


        // Functions
        Matrix4x4 identity();
        void setToIdentity();

        bool inverse();
        void transpose();

        void translateX(GLfloat x = 0.f);
        void translateY(GLfloat y = 0.f);
        void translateZ(GLfloat z = 0.f);
        void translate(GLfloat x = 0.f, GLfloat y = 0.f, GLfloat z = 0.f);
        void translate(Vector3D positionIn);

        void setPosition(GLfloat x = 0.f, GLfloat y = 0.f, GLfloat z = 0.f);
        Vector3D getPosition();

        void setRotationToVector(const Vector3D &direction, Vector3D up);

        // Rotate using EulerMatrix
        void rotateX(GLfloat degrees = 0.f);
        void rotateY(GLfloat degrees = 0.f);
        void rotateZ(GLfloat degrees = 0.f);

        // Rotate using Quaternion
        void qRotateX(GLfloat degrees = 0.f);
        void qRotateY(GLfloat degrees = 0.f);
        void qRotateZ(GLfloat degrees = 0.f);
        void rotate(Vector3D vector);

        // Rotate the matrix using an angle and an axis
        void rotate(GLfloat angle, const Vector3D &vector);
        void rotate(GLfloat angle, GLfloat xIn, GLfloat yIn, GLfloat zIn);

        void scale(Vector3D s);
        void scale(GLfloat uniformScale = 1.f);
        void scale(GLfloat scaleX = 1.f, GLfloat scaleY = 1.f, GLfloat scaleZ = 1.f);

        GLfloat* constData();

        void ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat nearPlane, GLfloat farPlane);
        void frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
        void perspective(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

        void lookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up_axis);

        Matrix2x2 toMatrix2();
        Matrix3x3 toMatrix3();

        friend std::ostream &operator<<(std::ostream &output, const Matrix4x4 &mIn)
        {
            output << std::setprecision(4) <<
                      "{" << mIn.matrix[0] << "\t, " << mIn.matrix[4] << "\t, " << mIn.matrix[8] << "\t, " << mIn.matrix[12] << "}\n" <<
                      "{" << mIn.matrix[1] << "\t, " << mIn.matrix[5] << "\t, " << mIn.matrix[9] << "\t, " << mIn.matrix[13] << "}\n" <<
                      "{" << mIn.matrix[2] << "\t, " << mIn.matrix[6] << "\t, " << mIn.matrix[10] << "\t, " << mIn.matrix[14] << "}\n" <<
                      "{" << mIn.matrix[3] << "\t, " << mIn.matrix[7] << "\t, " << mIn.matrix[11] << "\t, " << mIn.matrix[15] << "}\n";
            return output;
        }

    private:
        GLfloat matrix[16];
    };

} //namespace

#endif // MATRIX4X4_H
