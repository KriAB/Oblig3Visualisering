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
#include "FSM/npc.h"

SystemManager::SystemManager(RenderWindow *renderwindow, MainWindow *mainwindow, Shader *shaderProgram): mMainWindow(mainwindow), mRenderWindow(renderwindow),
    mShaderProgram{shaderProgram}
{
    mShaderProgram[0] = &shaderProgram[0];
    mShaderProgram[1] = &shaderProgram[1];
    mShaderProgram[2] = &shaderProgram[2];
    mShaderProgram[3] = &shaderProgram[3];
    mComponentSystem = new ComponentSystem(mRenderWindow, mMainWindow,*mShaderProgram, this);
    mCollisionSystem = new CollisionSystem(mComponentSystem);
    mInputSystem = new InputSystem(mComponentSystem);
    mRenderSystem = new RenderSystem(mComponentSystem);
    mTransformSystem = new TransformSystem(mComponentSystem);



    if(mComponentSystem->getInputComponent().at(0)!= nullptr)
        player = mComponentSystem->getInputComponent().at(0)->eID;


    setItemsPosition();
    mNPC = new NPC(endPts ,itemsPosition);
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
        //sjekker om det er noen input for spilleren
        checkPlayerInput();
        //Kalkulerer høyden etter hvor spilleren er på planet.
        mComponentSystem->updateHeightAndBarycentricCheck(player);

        //Regner ut bevegelsene til NPC og sjekker kollisjon på items
        calcNPCBehavior();
        //Passer på at NPC sin høyde blir riktig i forhold til plassering på planet.
         mComponentSystem->updateHeightAndBarycentricCheck(numNPC);
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

void SystemManager::calcNPCBehavior()
{
        for(int i = 0; i< items.size(); i++)
        {
            if(checkCollision(player, items.at(i)->eID) == true)
            {
                //Collision
                collisionHandling(items.at(i)->eID,i);
                //Update the points in NPC:
                setItemsPosition();
                mNPC->updatePoints(endPts,itemsPosition);
            }
        }


}

bool SystemManager::checkCollision(int EID, int otherEID)
{
    return  mCollisionSystem->collisionCheck(EID, otherEID);
}

bool SystemManager::collisionHandling(int otherEID, int index)
{
    //Set render to the object to false
    mComponentSystem->getRenderCompWithEId(otherEID)->isRendering = false;
     //Set collision to false
    mComponentSystem->getCollCompWithEId(otherEID)->isColliding = false;
    // remove from items
    items.erase(items.begin()+index);

}

void SystemManager::setItemsPosition()
{
    itemsPosition.clear();
    for(auto it: items)
    {
        itemsPosition.push_back(it->position());
    }
}

void SystemManager::setEndPts(const std::array<gsl::Vector3D, 2> &value)
{
    endPts = value;
}

void SystemManager::setItems(const std::vector<TransformComponent *> &value)
{
    items = value;
}

void SystemManager::setPlayer(int value)
{
    player = value;
}

void SystemManager::setNPC(int value)
{
    numNPC = value;
}
