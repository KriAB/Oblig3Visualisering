#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "shader.h"

class ColorShader : public Shader
{
public:
    ColorShader(const std::string shaderName, const GLchar *geometryPath = nullptr);
    virtual ~ColorShader() override;
};

#endif // COLORSHADER_H
