#include "innpch.h"
#include "billboard.h"
#include "renderwindow.h"

BillBoard::BillBoard()
{
    mVertices.insert( mVertices.end(),
    {
                          // Positions            // Normals          //UVs
                          Vertex{gsl::Vector3D(-2.f, -2.f,  0.f),   gsl::Vector3D(0.0f, 0.0f, 1.0f),     gsl::Vector2D(0.f, 0.f)},       // Bottom Left
                          Vertex{gsl::Vector3D(2.f, -2.f,  0.f),   gsl::Vector3D(0.0f, 0.0f, 1.0f),     gsl::Vector2D(1.f, 0.f)},       // Bottom Right
                          Vertex{gsl::Vector3D(-2.f, 2.f,  0.f),   gsl::Vector3D(0.0f, 0.0f, 1.0f),     gsl::Vector2D(0.f, 1.f)},       // Top Left
                          Vertex{gsl::Vector3D(2.f,  2.f,  0.f),   gsl::Vector3D(0.0f, 0.0f, 1.0f),     gsl::Vector2D(1.f, 1.f)}       // Top Right
                      });

    mMatrix.setToIdentity();
}

void BillBoard::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0  );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same parameter list as abowe
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  8 * sizeof( GLfloat ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

gsl::Vector3D BillBoard::getNormal()
{
    gsl::Matrix3x3 tempMat = mMatrix.toMatrix3();

    normal = tempMat * gsl::Vector3D{0.f, 0.f, -1.f};
    normal.normalize();

    return normal;
}

void BillBoard::setConstantYUp(bool constantUp)
{
    mConstantYUp = constantUp;
}

void BillBoard::draw()
{
    //find direction between this and camera
    gsl::Vector3D direction{};
    if(mNormalVersion)
    {
        gsl::Vector3D camPosition = mMaterial.mShader->getCurrentCamera()->position();
        //cancel heigth info so billboard is allways upright:
        if(mConstantYUp)
            camPosition.setY(mMatrix.getPosition().y);
        direction = camPosition - gsl::Vector3D(mMatrix.getPosition());
    }
    else {
        gsl::Vector3D camDirection = mMaterial.mShader->getCurrentCamera()->forward();
        //cancel heigth info so billboard is allways upright:
        if(mConstantYUp)
            camDirection.setY(mMatrix.getPosition().y);
        direction = camDirection * -1;
    }

    direction.normalize();
    //set rotation to this direction
    mMatrix.setRotationToVector(direction, gsl::Vector3D(0,1,0));

    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray( mVAO );
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
