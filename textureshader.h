#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"

class TextureShader : public Shader
{
public:
    TextureShader(const std::string shaderName, const GLchar *geometryPath = nullptr);
    virtual ~TextureShader() override;

    void transmitUniformData(gsl::Matrix4x4 *modelMatrix, Material *material) override;

private:
    GLint objectColorUniform{-1};
    GLint textureUniform{-1};
};

#endif // TEXTURESHADER_H
