#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "components.h"
class Entity;
class Camera;
class MainWindow;

enum InputMovementTypes
{
    //Movement
    MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_W, MOVE_A, MOVE_S, MOVE_D,
    //Not in use
    NO_MOVEMENT

};
enum InputFireTypes
{
    FIRE_SPACE,  FIRE_LMB,  FIRE_LSHIFT,
    //Not in use
     NO_FIRE
};

class InputComponent : public Components
{
public:
    InputComponent(int EID, InputMovementTypes moveForward, InputMovementTypes moveBackward,
                   InputMovementTypes moveLeft, InputMovementTypes moveRight, InputFireTypes fire);

    InputComponent();

    void setInputs(InputMovementTypes moveForward, InputMovementTypes moveBackward,
              InputMovementTypes moveLeft, InputMovementTypes moveRight, InputFireTypes fire);

    void setMoveForward(const InputMovementTypes &moveForward);
    void setMoveBackward(const InputMovementTypes &moveBackward);
    void setMoveLeft(const InputMovementTypes &moveLeft);
    void setMoveRight(const InputMovementTypes &moveRight);
    void setFire(const InputFireTypes &fire);

    InputMovementTypes moveBackward() const;
    InputMovementTypes moveForward() const;
    InputMovementTypes moveLeft() const;
    InputMovementTypes moveRight() const;
    InputFireTypes fire() const;




private:

    Camera *mCurrentCamera{nullptr};
    Entity *mPlayer{nullptr};  //the controllable object  -  use WASDQE
    MainWindow *mMainWindow{nullptr};

    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};

    int mMouseXlast{0};
    int mMouseYlast{0};

    InputMovementTypes mMoveForward{InputMovementTypes::NO_MOVEMENT};
    InputMovementTypes mMoveBackward{InputMovementTypes::NO_MOVEMENT};
    InputMovementTypes mMoveLeft{InputMovementTypes::NO_MOVEMENT};
    InputMovementTypes mMoveRight{InputMovementTypes::NO_MOVEMENT};
    InputFireTypes mFire{InputFireTypes::NO_FIRE};
};

#endif // INPUTCOMPONENT_H
