#include "inputcomponent.h"
#include "camera.h"
#include "mainwindow.h"
#include "Entity.h"

InputComponent::InputComponent(int EID, InputMovementTypes moveForward, InputMovementTypes moveBackward, InputMovementTypes moveLeft, InputMovementTypes moveRight, InputFireTypes fire):
    Components (compType::INPUTCOMP), mMoveForward(moveForward), mMoveBackward(moveBackward), mMoveLeft(moveLeft), mMoveRight(moveRight), mFire(fire)
{
    eID = EID;
}

InputComponent::InputComponent()
{

}

void InputComponent::setInputs(InputMovementTypes moveForward, InputMovementTypes moveBackward, InputMovementTypes moveLeft, InputMovementTypes moveRight, InputFireTypes fire)
{
    mMoveForward = moveForward;
    mMoveBackward = moveBackward;
    mMoveLeft = moveLeft;
    mMoveRight = moveRight;
    mFire = fire;

}

void InputComponent::setMoveForward(const InputMovementTypes &moveForward)
{
    mMoveForward = moveForward;
}

void InputComponent::setMoveBackward(const InputMovementTypes &moveBackward)
{
    mMoveBackward = moveBackward;
}

InputMovementTypes InputComponent::moveLeft() const
{
    return mMoveLeft;
}

void InputComponent::setMoveLeft(const InputMovementTypes &moveLeft)
{
    mMoveLeft = moveLeft;
}

InputMovementTypes InputComponent::moveRight() const
{
    return mMoveRight;
}

void InputComponent::setMoveRight(const InputMovementTypes &moveRight)
{
    mMoveRight = moveRight;
}

InputFireTypes InputComponent::fire() const
{
    return mFire;
}

void InputComponent::setFire(const InputFireTypes &fire)
{
    mFire = fire;
}

InputMovementTypes InputComponent::moveBackward() const
{
    return mMoveBackward;
}

InputMovementTypes InputComponent::moveForward() const
{
    return mMoveForward;
}
