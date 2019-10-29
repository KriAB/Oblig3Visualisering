#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <vector>
#include "input.h"

class InputComponent;
class MainWindow;
class Entity;
class Camera;
class ComponentSystem;

class InputSystem
{
public:
    InputSystem(ComponentSystem *compSyst);

     void handleInput();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

      void setCameraSpeed(float value);


private:

    Input mInput;
    float mCameraSpeed{0.01f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};
    Camera *mCurrentCamera{nullptr};
    Entity *mPlayer{nullptr};  //the controllable object  -  use WASDQE
    MainWindow *mMainWindow{nullptr};
    std::vector <InputComponent *> mInputComponent{nullptr};

   ComponentSystem *mComponentSystem{nullptr};







};

#endif // INPUTSYSTEM_H
