#include "transformcomponent.h"

TransformComponent::TransformComponent(): Components (compType::TRANSFORM)
{
    mMatrix.setToIdentity();
}
//TODO fix i matrix
gsl::Vector3D TransformComponent::getScale()
{
    return gsl::Vector3D{0,0,0};
}

gsl::Vector3D TransformComponent::getRotation()
{
      return gsl::Vector3D{0,0,0};
}
