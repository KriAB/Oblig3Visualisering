#include "innpch.h"
#include "triangle.h"

Triangle::Triangle()
{

}

void Triangle::init(GLuint shader)
{

    GLfloat vertices[]{
        // Positions            // Colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Left
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // Bottom Right
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
    };

    mMatrix.setToIdentity();
    mMatrix.translate(-1.f, -1.f, 1.f);

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                6 * sizeof(GLfloat),  // stride
                (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same parameter list as abowe
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof( GLfloat ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    mMatrixUniform = glGetUniformLocation( shader, "matrix" );

    glBindVertexArray(0);
}

void Triangle::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    mMatrix.translate(.001f, .001f, -.001f);
}
