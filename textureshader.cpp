#include "innpch.h"
#include "textureshader.h"
#include "material.h"

TextureShader::TextureShader(const std::string shaderName, const GLchar *geometryPath)
    :Shader(shaderName, geometryPath)
{
    mMatrixUniform = glGetUniformLocation( program, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( program, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( program, "pMatrix" );
    objectColorUniform = glGetUniformLocation( program, "objectColor" );
    textureUniform = glGetUniformLocation(program, "textureSampler");
}

TextureShader::~TextureShader()
{
    qDebug() << "Deleting TextureShader";
}

void TextureShader::transmitUniformData(gsl::Matrix4x4 *modelMatrix, Material *material)
{
    Shader::transmitUniformData(modelMatrix);

    glUniform1i(textureUniform, material->mTextureUnit); //TextureUnit = 0 as default);
    glUniform3f(objectColorUniform, material->mObjectColor.x, material->mObjectColor.y, material->mObjectColor.z);
}
