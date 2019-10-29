#include "innpch.h"
#include "colorshader.h"

ColorShader::ColorShader(const std::string shaderName, const GLchar *geometryPath)
    :Shader(shaderName, geometryPath)
{
    mMatrixUniform = glGetUniformLocation( program, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( program, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( program, "pMatrix" );
}

ColorShader::~ColorShader()
{
    qDebug() << "Deleting ColorShader";
}
