#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H
#include "material.h"
#include "Components/components.h"
class Shader;

class MaterialComponent : public Components
{
public:
    MaterialComponent();
      // void receive(int message);
       void setShader(Shader *shader);
       Material &returnMaterial(){return mMaterial;}
       void setTextureUnit(const GLuint &textureUnit);
private:
       Material mMaterial;
};

#endif // MATERIALCOMPONENT_H
