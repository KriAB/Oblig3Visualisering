#ifndef QUATERNION_H
#define QUATERNION_H

#include "math_constants.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "gltypes.h"
#include "vector3d.h"

namespace gsl
{
    class Vector3D;

    class Quaternion
    {
    public:
        Quaternion();

        // From angle-axis
        Quaternion(GLfloat angle_deg, Vector3D axis);
        // From Euler angles
        Quaternion(GLfloat roll, GLfloat pitch, GLfloat yaw);
        Quaternion(const Vector3D& rollpitchyaw);
        // From quaternion
        Quaternion(GLfloat ox, GLfloat oy, GLfloat oz, GLfloat ow);

        // Multiply Quaternion with Quaternion to Add the total rotations
        Quaternion operator*(const Quaternion& q);
        GLfloat& operator[](const int index);

        //Operator overloading as friendfunction
        friend std::ostream &operator<<(std::ostream &output, const Quaternion &rhs )
        {
            output << "X = " << rhs.x << ", Y = " << rhs.y <<
                      ", Z = " << rhs.z << ", W = " << rhs.w;
            return output;
        }

        void normalize();
        GLfloat magnitude();

        Matrix4x4 toRotationMatrix4();
        Matrix3x3 toRotationMatrix3();

        void toEulerAngles(GLfloat& roll, GLfloat& pitch, GLfloat& yaw);
        Vector3D toEulerAngles();
    private:
        float x, y, z, w;
    };
}

#endif // QUATERNION_H
