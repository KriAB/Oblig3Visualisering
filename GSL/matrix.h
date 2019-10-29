#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <iostream>
//#include <QDebug>
#include <iomanip>

#include "vector3d.h"
#include "vector4d.h"
#include "math_constants.h"

namespace gsl
{
    template <size_t row, size_t col>
    class Matrix
    {
        // Type alias
        using Mat = Matrix<row, col>;
    public:
        // Constructors
        Matrix();
        Matrix(const int type);
        Matrix(std::initializer_list<GLfloat> list);

        // Operator overloading
        GLfloat &operator()(int m, int n);
        GLfloat operator()(int m, int n) const;
        Mat operator+(const Mat &rhs) const;        // m + m
        Mat operator-(const Mat &rhs) const;        // m - m
        Mat operator*(const GLfloat rhs) const;       // m * f
        template <size_t row1, size_t col1>         // f * m
        friend Matrix<row,col> operator*(const GLfloat lhs, const Matrix<row, col> &rhs);
        Vector4D operator*(const Vector4D &rhs);            // m * v4
        Vector3D operator*(const Vector3D &rhs);            // m * v3
        Mat operator*(const Mat &rhs) const;        // m * m
        Mat &operator*=(const Mat &rhs);            // m *= m

        // Transformations
        void identity();
        void scale(GLfloat x, GLfloat y, GLfloat z);
        void scale(const Vector3D &scale);
        void rotate(GLfloat degrees, const Vector3D &axis);
        void translate(GLfloat x, GLfloat y, GLfloat z);
        void translate(const Vector3D &pos);

        // Special matrices
        static Mat orthographic(GLfloat n, GLfloat f, GLfloat t, GLfloat r); // TODO fix
        static Mat perspective(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f);
        static Mat lookAt(const Vector3D &cameraPos, const Vector3D &target, const Vector3D &worldUp);

        // Calculations
        GLfloat determinant();
        bool inverse();

        // Find inverse using determinants, fastest method available
        // https://en.wikipedia.org/wiki/Invertible_matrix
        bool inverse2x2();
        bool inverse3x3();
        bool inverse4x4();

        void transpose();
        std::vector<GLfloat> solveLinearEquation();

        // Set and get
        void setMatrix(std::initializer_list<GLfloat> list);
        void setMatrix(const GLfloat *array);
        void setMatrix(const std::vector<GLfloat> &array);
        void setMatrix(const std::array<GLfloat,row*col> &array);
        const GLfloat *getArrayPtr() const;

        // Print matrix
        void print() const;
        template <size_t row1, size_t col1>
        friend std::ostream& operator<<(std::ostream &out, const Matrix<row, col> &rhs);
//        template <size_t row1, size_t col1>
//        friend QDebug operator<<(QDebug out, const Matrix<row, col> &rhs);


    private:
        void swap(int row1, int row2, std::vector<GLfloat> &array);
        void rowPlusRow(GLfloat K, int row1, int row2, std::vector<GLfloat> &array);
        void rowTimesConstant(GLfloat K, int row1, std::vector<GLfloat> &array);
        bool notZero(GLfloat k);
        void makeZero(std::vector<GLfloat> &array);
        void matrixMultiplication(const Mat &mat);

        std::vector<GLfloat> mMatrix;
    };

    //-------------------------------------------------------------------------------

    // Constructors
    template<size_t row, size_t col>
    Matrix<row,col>::Matrix()
    {
        mMatrix.resize(row*col);
        identity();
    }

    template<size_t row, size_t col>
    Matrix<row, col>::Matrix(const int type)
    {
        mMatrix.resize(row*col);
        if (type == 0)
            makeZero(mMatrix);
        else if (type == 1)
            identity();
        else
            std::cout << "ERROR::MATRIX - Not valid matrix type\n";
    }

    template<size_t row, size_t col>
    Matrix<row,col>::Matrix(std::initializer_list<GLfloat> list)
        : mMatrix{list}
    {
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::setMatrix(std::initializer_list<float> list)
    {
        mMatrix = list;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::setMatrix(const GLfloat* array)
    {
        for (int i = 0; i < (row*col); ++i)
            mMatrix[i] = array[i];
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::setMatrix(const std::vector<GLfloat>& array)
    {
        mMatrix = array;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::setMatrix(const std::array<GLfloat, row*col>& array)
    {
        std::copy(array.begin(), array.end(), mMatrix.begin());
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::identity()
    {
        makeZero(mMatrix);
        int size;
        if (row<col)
            size = row;
        else
            size = col;
        for (int mn = 0; mn < size; ++mn)
            mMatrix[mn * col + mn] = 1.0f;
    }

    // Functor overload to be able to access the 1d mMatrix like a 2d array.
    // AKA mat(0)(0) eq. mat[0][0] returns mat[0]
    template<size_t row, size_t col>
    GLfloat& Matrix<row,col>::operator()(int m, int n)
    {
        return mMatrix[m * col + n];
    }

    template<size_t row, size_t col>
    GLfloat Matrix<row, col>::operator()(int m, int n) const
    {
        return mMatrix[m * col + n];
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row, col>::operator+(const Mat &rhs) const
    {
        Mat result(0);

        unsigned int m, n;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n)
                result(m, n) = mMatrix[m * col + n] + rhs(m, n);
        }
        return result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row, col>::operator-(const Mat &rhs) const
    {
        Mat result(0);

        unsigned int m, n;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n)
                result(m, n) = mMatrix[m * col + n] - rhs(m, n);
        }
        return result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row,col>::operator*(const GLfloat rhs) const
    {
        Mat result(0);

        unsigned int m, n;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n)
                result(m, n) = mMatrix[m * col + n] * rhs;
        }
        return result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> operator*(const GLfloat lhs, const Matrix<row, col> &rhs)
    {
        Matrix<row, col> result(0);

        unsigned int m, n;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n)
                result(m, n) = lhs * rhs(m, n);
        }
        return result;
    }

    template<size_t row, size_t col>
    Vector4D Matrix<row,col>::operator*(const Vector4D &rhs)
    {
        assert(row == col == 4);

        Vector4D result;
        result.x = (mMatrix[0] * rhs.x
                    + mMatrix[1] * rhs.y
                    + mMatrix[2] * rhs.z
                    + mMatrix[3] * rhs.w);
        result.y = (mMatrix[col] * rhs.x
                    + mMatrix[col + 1] * rhs.y
                    + mMatrix[col + 2] * rhs.z
                    + mMatrix[col + 3] * rhs.w);
        result.z = (mMatrix[2 * col] * rhs.x
                    + mMatrix[2 * col + 1] * rhs.y
                    + mMatrix[2 * col + 2] * rhs.z
                    + mMatrix[2 * col + 3] * rhs.w);
        result.w = (mMatrix[3 * col] * rhs.x
                    + mMatrix[3 * col + 1] * rhs.y
                    + mMatrix[3 * col + 2] * rhs.z
                    + mMatrix[3 * col + 3] * rhs.w);
        return result;
    }

    template<size_t row, size_t col>
    Vector3D Matrix<row,col>::operator*(const Vector3D &rhs)
    {
        assert(row == col == 3);

        Vector3D result;
        result.x =   (mMatrix[0] * rhs.x
                    + mMatrix[1] * rhs.y
                    + mMatrix[2] * rhs.z);
        result.y =   (mMatrix[1] * rhs.x
                    + mMatrix[col + 1] * rhs.y
                    + mMatrix[col + 2] * rhs.z);
        result.z =   (mMatrix[2] * rhs.x
                    + mMatrix[2 * col + 1] * rhs.y
                    + mMatrix[2 * col + 2] * rhs.z);
        return result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row,col>::operator*(const Mat &rhs) const
    {
        Mat result(0);

        unsigned int m, n, i;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n) {
                for (i = 0; i < col; ++i)
                    result(m, n) += mMatrix[m * col + i] * rhs(i,n);
            }
        }
        return {result};
    }

    template<size_t row, size_t col>
    Matrix<row, col>& Matrix<row, col>::operator*=(const Mat &rhs)
    {
        matrixMultiplication(rhs);
        return *this;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::scale(GLfloat x, GLfloat y, GLfloat z)
    {
        Mat temp;
        temp(0, 0) = x;
        temp(1, 1) = y;
        temp(2, 2) = z;
        matrixMultiplication(temp);
    }

    template<size_t row, size_t col>
    inline void Matrix<row, col>::scale(const Vector3D & scale)
    {
        Mat temp;
        temp(0, 0) = scale.x;
        temp(1, 1) = scale.y;
        temp(2, 2) = scale.z;
        matrixMultiplication(temp);
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::rotate(GLfloat degrees, const Vector3D& u)
    {
        if (u.length() >= 0.1f) {
            GLfloat rad = degrees/(180.0f/PI);
            Mat rotationMat = {
                cos(rad) + pow(u.x, 2.0f) * (1.0f - cos(rad)),		u.x * u.y * (1.0f - cos(rad)) - u.z * sin(rad),		u.x * u.z * (1.0f - cos(rad)) + u.y * sin(rad),	 0.0f,
                u.y * u.x * (1.0f - cos(rad)) + u.z * sin(rad),		cos(rad) + pow(u.y, 2.0f) * (1.0f - cos(rad)),		u.y * u.z * (1.0f - cos(rad)) - u.x * sin(rad),	 0.0f,
                u.z * u.x * (1.0f - cos(rad)) - u.y * sin(rad),		u.z * u.y * (1.0f - cos(rad)) + u.x * sin(rad),		cos(rad) + pow(u.z, 2.0f) * (1.0f - cos(rad)),	 0.0f,
                0.0f,												0.0f,												0.0f,											 1.0f
            };
            matrixMultiplication(rotationMat);
        }
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::translate(GLfloat x, GLfloat y, GLfloat z)
    {
        Matrix<row,col> temp;
        temp.identity();
        temp(0, 3) = x;
        temp(1, 3) = y;
        temp(2, 3) = z;
        matrixMultiplication(temp);
    }

    template<size_t row, size_t col>
    void Matrix<row, col>::translate(const Vector3D& pos)
    {
        Matrix<row, col> temp;
        temp.identity();
        temp(0, 3) = pos.x;
        temp(1, 3) = pos.y;
        temp(2, 3) = pos.z;
        matrixMultiplication(temp);
    }

    template<size_t row, size_t col>
    GLfloat Matrix<row,col>::determinant()
    {
        auto copy = mMatrix;
        int timesSwapped = 0, rowToSwap, diag, j;

        GLfloat k;

        for (diag = 0; diag < row; diag++) {
            // Bytter om rader hvis en av radene under diagonalen har en større ledende koeffisient
            rowToSwap = 0;
            for (j = diag + 1; j < row; j++) {
                if (copy[j * col + diag]>copy[diag * col + diag])
                    rowToSwap = j;
            }

            if (rowToSwap>diag) {
                swap(diag, rowToSwap, copy);
                ++timesSwapped;
            }
            // Gjør ledende koeffisienter under diagonalen til null
            for (j = diag + 1; j < row; j++) {
                if (notZero(copy[j * col + diag])) {
                    k = copy[j * col + diag] / copy[diag * col + diag];
                    rowPlusRow(-k, diag, j, copy);
                }
            }
        }
        // Multipliserer diagonalen
        GLfloat result = 1.0;
        for (int i = 0; i < row; i++)
            result *= copy[i * col + i];
        // For hver gang vi bytter rad edres fortegn
        result *= pow(-1, timesSwapped);
        return result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row,col>::orthographic(GLfloat n, GLfloat f, GLfloat t, GLfloat r)
    {
        Mat result(0);
        result(0, 0) = 1/r;
        result(1, 1) = 1/t;
        result(2, 2) = -2/(f-n);
        result(2, 3) = -((f+n)/(f-n));
        result(3, 3) = 1;
        return  result;
    }

    template<size_t row, size_t col>
    Matrix<row, col> Matrix<row,col>::perspective(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f)
    {
        GLfloat tanHalfFov = tan(fov / 2.0f);

        Mat result(0);
        result(0,0) = 1.0f / (aspect * tanHalfFov);
        result(1,1) = 1.0f / tanHalfFov;
        result(2,2) = -(f + n) / (f - n);
        result(3,2) = -1.0f;
        result(2,3) = -(2.0f * f * n) / (f - n);
        return result;
    }


    //OEF: funksjonene fra Vec3-klassen blir ikke sett av denne klassen, så kommenterer ut funksjonen inntil videre
//    template<size_t row, size_t col>
//    Matrix<row, col> Matrix<row, col>::lookAt(const Vec3 &cameraPos, const Vec3 &target, const Vec3 &worldUp)
//    {
//        //OEF: finner ikke normalize-funksjonen, selv om jeg har Vec3.normalize
////        Vec3 front = normalize(cameraPos - target);
//        Vec3 front = cameraPos - target;
//        front.normalize();

//        //OEF: samme med disse
//        Vec3 right = normalize(cross(front, -worldUp));
//        Vec3 up = cross(front, right);

//        Mat a =
//        {
//            right.x,		right.y,		right.z,		0.0f,
//            up.x,			up.y,			up.z,			0.0f,
//            front.x,		front.y,		front.z,		0.0f,
//            0.0f,           0.0f,           0.0f,           1.0f
//        };

//        Mat b;
//        b.translate(-cameraPos);
//        return { a * b };
//    }

    template<size_t row, size_t col>
    std::vector<GLfloat> Matrix<row,col>::solveLinearEquation()
    {
        auto copy = mMatrix;
        int cycle = 0, i, j;

        GLfloat k;

        for (i = 0; i < row; ++i) {
            for (j = cycle; j < row; ++j) {
                if (notZero(copy[j * col + cycle])) {
                    if (j>cycle) {
                        swap(cycle, j, copy);
                    }
                    k = 1/copy[cycle * col + cycle];
                    rowTimesConstant(k, cycle, copy);
                    break;
                }
            }
            for (int j = 0; j < row; ++j) {
                if (notZero(copy[j * col + cycle]) && j!=cycle) {
                    k = -(copy[j * col + cycle]);
                    rowPlusRow(k, cycle, j, copy);
                }
            }
            ++cycle;
        }
        std::vector<GLfloat> Result;
        for (int i = 0; i < row; ++i)
            Result.push_back(copy[i * col + col-1]);
        return Result;
    }

    // LMN: Needs to be a lot faster than this, see: inverse matrix from determinant.
    // matrix4x4.cpp inverse as example
    template<size_t row, size_t col>
    bool Matrix<row,col>::inverse()
    {
        if(row == col)   // Makes no sense trying to find inverse using gaussian method if it is quadratic
        {
            switch(row) // No breaks needed because we return on every case
            {
            case 2:
                return inverse2x2();
            case 3:
                return inverse3x3();
            case 4:
                return inverse4x4();
            }
        }

        // This must be true if we are finding the inverse based on gaussian elimination
        assert(col == row + 1);

        GLfloat det = determinant();

        // Inverse has no solution if the determinant == 0
        if(det == 0.f)
            return false;

        // Do the following if the matrix has n > 4
        auto copy = mMatrix;
        identity();
        int cycle = 0, i, j;

        GLfloat k;

        for (i = 0; i < col; ++i) {
            for (j = cycle; j < row; ++j) {
                if (notZero(copy[j * col + cycle])) {
                    if (j>cycle) {
                        swap(cycle, j, copy);
                        swap(cycle, j, mMatrix);
                    }
                    k = 1/copy[cycle * col + cycle];
                    rowTimesConstant(k, cycle, copy);
                    rowTimesConstant(k, cycle, mMatrix);
                    break;
                }
            }
            for (j = 0; j < row; ++j) {
                if (notZero(copy[j * col + cycle]) && j!=cycle) {
                    k = -(copy[j * col + cycle]);
                    rowPlusRow(k, cycle, j, copy);
                    rowPlusRow(k, cycle, j, mMatrix);
                }
            }
            ++cycle;
        }

        return true;
    }

    template<size_t row, size_t col>
    bool Matrix<row, col>::inverse2x2()
    {
        assert(row == 2 && col == 2);

        Matrix temp =
        {
            (*this)(1, 1), -(*this)(0, 1),
            -(*this)(1, 0), (*this)(0, 0)
        };

        GLfloat det = determinant();

        if(det == 0.f)
            return false;

        *this = (1.f / det) * temp;

        return true;
    }

    template<size_t row, size_t col>
    bool Matrix<row, col>::inverse3x3()
    {
        assert(row == 3 && col == 3);

        GLfloat a = (*this)(0, 0), b = (*this)(0, 1), c = (*this)(0, 2),
                d = (*this)(1, 0), e = (*this)(1, 1), f = (*this)(1, 2),
                g = (*this)(2, 0), h = (*this)(2, 1), i = (*this)(2, 2);

        GLfloat A =  (e*i-f*h), B = -(d*i-f*g), C =  (d*h-e*g),
                D = -(b*i-c*h), E =  (a*i-c*g), F = -(a*h-b*g),
                G =  (b*f-c*e), H = -(a*f-c*d), I =  (a*e-b*d);

        GLfloat det = determinant();

        if(det == 0.f)
            return false;

        Matrix temp =
        {
            A, D, G,
            B, E, H,
            C, F, I
        };

        *this = (1.f / det) * temp;
        return true;
    }

    template<size_t row, size_t col>
    bool Matrix<row, col>::inverse4x4()
    {
        assert(row == 4 && col == 4);
        GLfloat inv[16], det;
        GLfloat invOut[16];

        int i;

        inv[0] = mMatrix[5]  * mMatrix[10] * mMatrix[15] -
                mMatrix[5]  * mMatrix[11] * mMatrix[14] -
                mMatrix[9]  * mMatrix[6]  * mMatrix[15] +
                mMatrix[9]  * mMatrix[7]  * mMatrix[14] +
                mMatrix[13] * mMatrix[6]  * mMatrix[11] -
                mMatrix[13] * mMatrix[7]  * mMatrix[10];

        inv[4] = -mMatrix[4]  * mMatrix[10] * mMatrix[15] +
                mMatrix[4]  * mMatrix[11] * mMatrix[14] +
                mMatrix[8]  * mMatrix[6]  * mMatrix[15] -
                mMatrix[8]  * mMatrix[7]  * mMatrix[14] -
                mMatrix[12] * mMatrix[6]  * mMatrix[11] +
                mMatrix[12] * mMatrix[7]  * mMatrix[10];

        inv[8] = mMatrix[4]  * mMatrix[9] * mMatrix[15] -
                mMatrix[4]  * mMatrix[11] * mMatrix[13] -
                mMatrix[8]  * mMatrix[5] * mMatrix[15] +
                mMatrix[8]  * mMatrix[7] * mMatrix[13] +
                mMatrix[12] * mMatrix[5] * mMatrix[11] -
                mMatrix[12] * mMatrix[7] * mMatrix[9];

        inv[12] = -mMatrix[4]  * mMatrix[9] * mMatrix[14] +
                mMatrix[4]  * mMatrix[10] * mMatrix[13] +
                mMatrix[8]  * mMatrix[5] * mMatrix[14] -
                mMatrix[8]  * mMatrix[6] * mMatrix[13] -
                mMatrix[12] * mMatrix[5] * mMatrix[10] +
                mMatrix[12] * mMatrix[6] * mMatrix[9];

        inv[1] = -mMatrix[1]  * mMatrix[10] * mMatrix[15] +
                mMatrix[1]  * mMatrix[11] * mMatrix[14] +
                mMatrix[9]  * mMatrix[2] * mMatrix[15] -
                mMatrix[9]  * mMatrix[3] * mMatrix[14] -
                mMatrix[13] * mMatrix[2] * mMatrix[11] +
                mMatrix[13] * mMatrix[3] * mMatrix[10];

        inv[5] = mMatrix[0]  * mMatrix[10] * mMatrix[15] -
                mMatrix[0]  * mMatrix[11] * mMatrix[14] -
                mMatrix[8]  * mMatrix[2] * mMatrix[15] +
                mMatrix[8]  * mMatrix[3] * mMatrix[14] +
                mMatrix[12] * mMatrix[2] * mMatrix[11] -
                mMatrix[12] * mMatrix[3] * mMatrix[10];

        inv[9] = -mMatrix[0]  * mMatrix[9] * mMatrix[15] +
                mMatrix[0]  * mMatrix[11] * mMatrix[13] +
                mMatrix[8]  * mMatrix[1] * mMatrix[15] -
                mMatrix[8]  * mMatrix[3] * mMatrix[13] -
                mMatrix[12] * mMatrix[1] * mMatrix[11] +
                mMatrix[12] * mMatrix[3] * mMatrix[9];

        inv[13] = mMatrix[0]  * mMatrix[9] * mMatrix[14] -
                mMatrix[0]  * mMatrix[10] * mMatrix[13] -
                mMatrix[8]  * mMatrix[1] * mMatrix[14] +
                mMatrix[8]  * mMatrix[2] * mMatrix[13] +
                mMatrix[12] * mMatrix[1] * mMatrix[10] -
                mMatrix[12] * mMatrix[2] * mMatrix[9];

        inv[2] = mMatrix[1]  * mMatrix[6] * mMatrix[15] -
                mMatrix[1]  * mMatrix[7] * mMatrix[14] -
                mMatrix[5]  * mMatrix[2] * mMatrix[15] +
                mMatrix[5]  * mMatrix[3] * mMatrix[14] +
                mMatrix[13] * mMatrix[2] * mMatrix[7] -
                mMatrix[13] * mMatrix[3] * mMatrix[6];

        inv[6] = -mMatrix[0]  * mMatrix[6] * mMatrix[15] +
                mMatrix[0]  * mMatrix[7] * mMatrix[14] +
                mMatrix[4]  * mMatrix[2] * mMatrix[15] -
                mMatrix[4]  * mMatrix[3] * mMatrix[14] -
                mMatrix[12] * mMatrix[2] * mMatrix[7] +
                mMatrix[12] * mMatrix[3] * mMatrix[6];

        inv[10] = mMatrix[0]  * mMatrix[5] * mMatrix[15] -
                mMatrix[0]  * mMatrix[7] * mMatrix[13] -
                mMatrix[4]  * mMatrix[1] * mMatrix[15] +
                mMatrix[4]  * mMatrix[3] * mMatrix[13] +
                mMatrix[12] * mMatrix[1] * mMatrix[7] -
                mMatrix[12] * mMatrix[3] * mMatrix[5];

        inv[14] = -mMatrix[0]  * mMatrix[5] * mMatrix[14] +
                mMatrix[0]  * mMatrix[6] * mMatrix[13] +
                mMatrix[4]  * mMatrix[1] * mMatrix[14] -
                mMatrix[4]  * mMatrix[2] * mMatrix[13] -
                mMatrix[12] * mMatrix[1] * mMatrix[6] +
                mMatrix[12] * mMatrix[2] * mMatrix[5];

        inv[3] = -mMatrix[1] * mMatrix[6] * mMatrix[11] +
                mMatrix[1] * mMatrix[7] * mMatrix[10] +
                mMatrix[5] * mMatrix[2] * mMatrix[11] -
                mMatrix[5] * mMatrix[3] * mMatrix[10] -
                mMatrix[9] * mMatrix[2] * mMatrix[7] +
                mMatrix[9] * mMatrix[3] * mMatrix[6];

        inv[7] = mMatrix[0] * mMatrix[6] * mMatrix[11] -
                mMatrix[0] * mMatrix[7] * mMatrix[10] -
                mMatrix[4] * mMatrix[2] * mMatrix[11] +
                mMatrix[4] * mMatrix[3] * mMatrix[10] +
                mMatrix[8] * mMatrix[2] * mMatrix[7] -
                mMatrix[8] * mMatrix[3] * mMatrix[6];

        inv[11] = -mMatrix[0] * mMatrix[5] * mMatrix[11] +
                mMatrix[0] * mMatrix[7] * mMatrix[9] +
                mMatrix[4] * mMatrix[1] * mMatrix[11] -
                mMatrix[4] * mMatrix[3] * mMatrix[9] -
                mMatrix[8] * mMatrix[1] * mMatrix[7] +
                mMatrix[8] * mMatrix[3] * mMatrix[5];

        inv[15] = mMatrix[0] * mMatrix[5] * mMatrix[10] -
                mMatrix[0] * mMatrix[6] * mMatrix[9] -
                mMatrix[4] * mMatrix[1] * mMatrix[10] +
                mMatrix[4] * mMatrix[2] * mMatrix[9] +
                mMatrix[8] * mMatrix[1] * mMatrix[6] -
                mMatrix[8] * mMatrix[2] * mMatrix[5];

        det = mMatrix[0] * inv[0] + mMatrix[1] * inv[4] + mMatrix[2] * inv[8] + mMatrix[3] * inv[12];

        if (det == 0.f)
            return false;

        det = 1.f / det;

        for (i = 0; i < 16; i++)
            invOut[i] = inv[i] * det;

        for(int i = 0; i < 16; i++)
            mMatrix[i] = invOut[i];

        return true;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::transpose()
    {
        Mat copy{*this};
        unsigned int m, n;
        for (m = 0; m < row; m++) {
            for (n = 0; n < col; n++)
                mMatrix[m * col + n] = copy(n, m);
        }
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::print() const
    {
        int i, j;
        for (i = 0; i < row; ++i) {
            std::cout << "(";
            for (j = 0; j < col; ++j)
                std::cout << mMatrix[i * col + j] << " ";
            std::cout << ")\n";
        }
    }

    template<size_t row, size_t col>
    const GLfloat *Matrix<row, col>::getArrayPtr() const
    {
        return mMatrix.data();
    }

    template<size_t row, size_t col>
    std::ostream &operator<<(std::ostream &out, const Matrix<row, col> &rhs)
    {
        int i, j;
        for (i = 0; i < row; ++i) {
            out << "(";
            for (j = 0; j < col; ++j)
                out << rhs(i, j) << ((j<col-1)?" ":"");
            out << ")\n";
        }
        return out;
    }

    //OEF: lager uavhengighet fra Qt som en start
//    template <size_t row, size_t col>
//    QDebug operator<<(QDebug out, const Matrix<row, col> &rhs)
//    {
//        out << "\n";
//        for (int i = 0; i < row; ++i) {
//            out << "(";
//            for (int j = 0; j < col; ++j)
//                out << rhs(i, j) << ((j<col-1)?" ":"");
//            out << ")\n";
//        }
//        return out;
//    }

    template<size_t row, size_t col>
    void Matrix<row,col>::swap(int row1, int row2, std::vector<GLfloat> &array)
    {
        auto temp = array;
        for (unsigned int i = 0; i < col; ++i) {
            array[row1 * col + i] = temp[row2 * col + i];
            array[row2 * col + i] = temp[row1 * col + i];
        }
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::rowPlusRow(GLfloat k, int row1, int row2, std::vector<GLfloat> &array)
    {
        for (unsigned int i = 0; i < col; ++i)
            array[row2 * col + i] += array[row1 * col + i] * k;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::rowTimesConstant(GLfloat k, int row1, std::vector<GLfloat> &array)
    {
        for (unsigned int i = 0; i < col; ++i)
            array[row1 * col + i] *= k;
    }

    template<size_t row, size_t col>
    bool Matrix<row,col>::notZero(GLfloat k)
    {
        if (abs(k) < 0.000001f)
            return false;
        return true;
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::makeZero(std::vector<GLfloat> &array)
    {
        std::fill(array.begin(), array.end(), 0.0f);
    }

    template<size_t row, size_t col>
    void Matrix<row,col>::matrixMultiplication(const Mat& mat)
    {
        Mat result(0);

        unsigned int m, n, i;

        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n) {
                for (i = 0; i < col; ++i)
                    result(m, n) += mat(i, n) * mMatrix[m * col + i];
            }
        }
        for (m = 0; m < row; ++m) {
            for (n = 0; n < col; ++n)
                mMatrix[m * col + n] = result(m, n);
        }
    }
}

#endif // MATRIX_H
