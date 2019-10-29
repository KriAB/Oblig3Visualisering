#include "innpch.h"
#include "octahedronball.h"

OctahedronBall::OctahedronBall(GLint n) : mRecursions{n}, mIndex{0}
{
    mNumberOfVertices = static_cast<GLuint>(3 * 8 * std::pow(4, mRecursions));
    mMatrix.setToIdentity();
    makeUnitOctahedron();
}

OctahedronBall::~OctahedronBall()
{
}

void OctahedronBall::makeTriangle(const gsl::Vector3D &v1, const gsl::Vector3D &v2, const gsl::Vector3D &v3)
{
    mVertices.push_back(Vertex(v1, v1, gsl::Vector2D(0.f, 0.f)));
    mVertices.push_back(Vertex(v2, v2, gsl::Vector2D(1.f, 0.f)));
    mVertices.push_back(Vertex(v3, v3, gsl::Vector2D(0.5f, 1.f)));

}

void OctahedronBall::subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, GLint n)
{
    if (n > 0)
    {
        gsl::Vector3D v1 = a + b;
        v1.normalize();

        gsl::Vector3D v2 = a + c;
        v2.normalize();

        gsl::Vector3D v3 = c + b;
        v3.normalize();

        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    }
    else
    {
        makeTriangle(a, b, c);
    }
}

void OctahedronBall::makeUnitOctahedron()
{
    gsl::Vector3D v0(0.f, 0.f, 1.f);
    gsl::Vector3D v1(1.f, 0.f, 0.f);
    gsl::Vector3D v2(0.f, 1.f, 0.f);
    gsl::Vector3D v3(-1.f, 0.f, 0.f);
    gsl::Vector3D v4(0.f, -1.f, 0.f);
    gsl::Vector3D v5(0.f, 0.f, -1.f);

    subDivide(v0, v1, v2, mRecursions);
    subDivide(v0, v2, v3, mRecursions);
    subDivide(v0, v3, v4, mRecursions);
    subDivide(v0, v4, v1, mRecursions);
    subDivide(v5, v2, v1, mRecursions);
    subDivide(v5, v3, v2, mRecursions);
    subDivide(v5, v4, v3, mRecursions);
    subDivide(v5, v1, v4, mRecursions);
}

//OctahedronBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//  for using later use of glDrawArrays()
void OctahedronBall::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void OctahedronBall::draw()
{
    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray( mVAO );
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
    glBindVertexArray(0);
}

