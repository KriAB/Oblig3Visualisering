#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle :  protected QOpenGLFunctions_4_1_Core
{
public:
    Triangle();

    void draw();
    void init(GLuint shader);

private:
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLint  mMatrixUniform{0};
    gsl::Matrix4x4 mMatrix;
};

#endif // TRIANGLE_H
