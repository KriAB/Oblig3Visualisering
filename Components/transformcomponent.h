#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "components.h"
#include "innpch.h"

class TransformComponent : public Components
{

    gsl::Matrix4x4 mMatrix;
public:
    TransformComponent();
    gsl::Matrix4x4 &matrix(){return mMatrix;}
    void setMatrix(gsl::Matrix4x4 mat){mMatrix = mat;}
    gsl::Vector3D position() {return mMatrix.getPosition();}
    gsl::Vector3D getScale();
    gsl::Vector3D getRotation();

    void setPosition(gsl::Vector3D newPos){ mMatrix.setPosition(newPos.x,newPos.y,newPos.z);}
    void translate(gsl::Vector3D translate){mMatrix.translate(translate);}
    void scale(float mScale){mMatrix.scale(gsl::Vector3D(mScale,mScale,mScale));}
    void rotate();

    //Lagre start transform til objektet. Hvis man skal pause og play, sånn at man kan sette det tilbake.

    gsl::Matrix4x4 mTranslateMatrix;
    gsl::Matrix4x4 mScaleMatrix;
    gsl::Matrix4x4 mRotationMatrix;


};

#endif // TRANSFORMCOMPONENT_H
