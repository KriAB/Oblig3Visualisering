#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "components.h"
class Entity;
class Camera;
class MainWindow;

class InputComponent : public Components
{
public:
    InputComponent();

private:

    Camera *mCurrentCamera{nullptr};
    Entity *mPlayer{nullptr};  //the controllable object  -  use WASDQE
    MainWindow *mMainWindow{nullptr};

    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};

    int mMouseXlast{0};
    int mMouseYlast{0};
};

#endif // INPUTCOMPONENT_H
