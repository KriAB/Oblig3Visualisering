#include "systemmanager.h"

#include "inputsystem.h"
#include "componentsystem.h"
#include "collisionsystem.h"
#include "rendersystem.h"

#include "renderwindow.h"
#include "shader.h"
#include "mainwindow.h"

SystemManager::SystemManager(RenderWindow *renderwindow, MainWindow *mainwindow, Shader *shaderProgram): mMainWindow(mainwindow), mRenderWindow(renderwindow),
    mShaderProgram{shaderProgram}
{
    mShaderProgram[0] = &shaderProgram[0];
    mShaderProgram[1] = &shaderProgram[1];
    mShaderProgram[2] = &shaderProgram[2];
    mShaderProgram[3] = &shaderProgram[3];
    mComponentSystem = new ComponentSystem(mRenderWindow, mMainWindow,*mShaderProgram);
    mCollisionSystem = new CollisionSystem(mComponentSystem);
    mInputSystem = new InputSystem(mComponentSystem);
    mRenderSystem = new RenderSystem(mComponentSystem);
}

SystemManager::~SystemManager()
{
    delete mCollisionSystem;
    mCollisionSystem = nullptr;

    delete mInputSystem;
    mInputSystem = nullptr;

    delete mRenderSystem;
    mRenderSystem = nullptr;

    delete  mComponentSystem;
    mComponentSystem = nullptr;
}

void SystemManager::update()
{
     mRenderSystem->update();
}

ComponentSystem *SystemManager::componentSystem()
{
    return mComponentSystem;
}

RenderSystem *SystemManager::renderSystem() const
{
    return mRenderSystem;
}
