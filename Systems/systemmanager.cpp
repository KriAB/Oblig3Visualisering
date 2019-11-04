#include "systemmanager.h"

#include "inputsystem.h"
#include "componentsystem.h"
#include "collisionsystem.h"
#include "rendersystem.h"
#include "transformsystem.h"
#include "inputcomponent.h"
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
    mTransformSystem = new TransformSystem(mComponentSystem);

    if(mComponentSystem->getInputComponent().at(0)!= nullptr)
        player = mComponentSystem->getInputComponent().at(0)->eID;
}

SystemManager::~SystemManager()
{
    delete mCollisionSystem;
    mCollisionSystem = nullptr;

    delete mInputSystem;
    mInputSystem = nullptr;

    delete mRenderSystem;
    mRenderSystem = nullptr;

    delete mTransformSystem;
    mTransformSystem = nullptr;

    delete  mComponentSystem;
    mComponentSystem = nullptr;
}

void SystemManager::update()
{
    if(mMainWindow->isPlaying)
    {
        checkPlayerInput();
        mComponentSystem->updateHeightAndBarycentricCheck(player);
        intervalCalculationHeight++;
    }
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

void SystemManager::checkPlayerInput()
{
    if(player != -1)
    {
        gsl::Vector3D playerPos{0,0,0};
        float playerSpeed{0.02};
        if(mComponentSystem->getInputCompWithEId(player)->moveForward() == InputMovementTypes::MOVE_UP)
        {
            if(mRenderWindow->getInput().UP == true)
            {
                playerPos.z = -playerSpeed;
            }

        }
        if(mComponentSystem->getInputCompWithEId(player)->moveBackward() == InputMovementTypes::MOVE_DOWN)
        {
            if(mRenderWindow->getInput().DOWN == true)

            {
                playerPos.z = playerSpeed;

            }

        }
        if(mComponentSystem->getInputCompWithEId(player)->moveLeft() == InputMovementTypes::MOVE_LEFT)
        {
            if(mRenderWindow->getInput().LEFT == true)
            {
                playerPos.x = -playerSpeed;

            }
        }

        if(mComponentSystem->getInputCompWithEId(player)->moveRight() == InputMovementTypes::MOVE_RIGHT)
        {
            if(mRenderWindow->getInput().RIGHT == true)
            {
                playerPos.x = playerSpeed;

            }
        }

        mComponentSystem->move(player,playerPos);
    }
}
