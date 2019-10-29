#include "materialcomponent.h"
#include "shader.h"

MaterialComponent::MaterialComponent() : Components (compType::MATERIAL)
{

}


void MaterialComponent::setShader(Shader *shader)
{
     mMaterial.mShader = shader;
}

void MaterialComponent::setTextureUnit(const GLuint &textureUnit)
{
    mMaterial.setTextureUnit(textureUnit);
}
