#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <vector>

class RenderSystem;
class ComponentSystem;
class InputSystem;
class CollisionSystem;
class TransformSystem;

class Shader;
class MainWindow;
class RenderWindow;


class SystemManager
{
public:
    SystemManager(RenderWindow *renderwindow = nullptr, MainWindow *mainwindow = nullptr, Shader *shaderProgram = nullptr);
     ~ SystemManager();
    void update();

    ComponentSystem *componentSystem();

    RenderSystem *renderSystem() const;

private:
    RenderSystem * mRenderSystem{nullptr};
    ComponentSystem * mComponentSystem{nullptr};
    InputSystem *mInputSystem{nullptr};
    CollisionSystem * mCollisionSystem{nullptr};
    TransformSystem * mTransformSystem{nullptr};

    MainWindow *mMainWindow{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    Shader *mShaderProgram[4]{nullptr};

};

#endif // SYSTEMMANAGER_H
