#include "componentsystem.h"
#include "resourcefactory.h"
#include "Scenes/scene.h"
#include "renderwindow.h"
#include "mainwindow.h"
#include "shader.h"

ComponentSystem::ComponentSystem(RenderWindow *renderwindow, MainWindow *mainWindow, Shader * shaderProgram) : mRenderWindow(renderwindow), mMainWindow(mainWindow),   mShaderProgram{shaderProgram}
{
    mResourceFactory = new ResourceFactory;
    makeScene();
    mScene = new Scene(this);
}

ComponentSystem::~ComponentSystem()
{
    delete  mResourceFactory;
    mResourceFactory = nullptr;

    for(auto ent: mEntities)
    {
        delete ent;
        ent = nullptr;
    }
    for(auto temp: mCollisionComponents)
    {
        delete temp;
        temp = nullptr;
    }
    for(auto temp: mRenderComponents)
    {
        delete temp;
        temp = nullptr;
    }
    for(auto temp: mTransformComponents)
    {
        delete temp;
        temp = nullptr;
    }
    for(auto temp: mMaterialComponents)
    {
        delete temp;
        temp = nullptr;
    }
    for(auto temp: mMeshComponents)
    {
        delete temp;
        temp = nullptr;
    }
}

void ComponentSystem::sortByEID()
{

}

void ComponentSystem::saveScene(std::string name)
{
       mScene->writeScene(name);
}

void ComponentSystem::loadScene(std::string name)
{
    mScene->openSceneFromFile(name);
}

void ComponentSystem::makeScene()
{
    mResourceFactory->mRenderWindow = mRenderWindow;

    mEntities.push_back(new Entity(0));
    mMeshComponents.push_back( mResourceFactory->makeResource("Cube"));
    mMaterialComponents.push_back(mResourceFactory->setMatComponent(mShaderProgram[0]));
    mTransformComponents.push_back(mResourceFactory->setTransComponent(gsl::Vector3D(0,0,0),0));
    mCollisionComponents.push_back(mResourceFactory->makeCollisionComponent("OBB",0));
    mRenderComponents.push_back(new RenderComponent(0,mMeshComponents.at(0),mMaterialComponents.at(0),mTransformComponents.at(0)));
    mMainWindow->addEntityInWorldContentList(0);
    numEntity++;

    mEntities.push_back(new Entity(1));
    mMeshComponents.push_back(mResourceFactory->makeResource("Cube"));
    mTransformComponents.push_back( mResourceFactory->setTransComponent(gsl::Vector3D(0,1,0),0));
    mMaterialComponents.push_back( mResourceFactory->setMatComponent(mShaderProgram[0]));
    mCollisionComponents.push_back( mResourceFactory->makeCollisionComponent("OBB",1));
    mRenderComponents.push_back(new RenderComponent(1,mMeshComponents.at(1),mMaterialComponents.at(1),mTransformComponents.at(1)));
    mMainWindow->addEntityInWorldContentList(1);

    //mResourceFactory->addChild(0,1);
    numEntity++;

}



//Skjer i factory
//void ComponentSystem::init()
//{

//}

//void ComponentSystem::update()
//{
//    //translateSystem->update();
//    //physicsSystem->update();
//    mRenderSystem->update();
//}

TransformComponent *ComponentSystem::getTransCompWithEId(int EID)
{
    //kunne hatt en array med indeksene til trans komponent. Dyrt å søke igjennom.
    for(auto temp : mTransformComponents)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

MeshComponent *ComponentSystem::getMeshCompWithEId(int EID)
{
    for(auto temp : mMeshComponents)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

CollisionComponent *ComponentSystem::getCollCompWithEId(int EID)
{
    for(auto temp : mCollisionComponents)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

MaterialComponent *ComponentSystem::getMatCompWithEId(int EID)
{
    for(auto temp : mMaterialComponents)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

SoundComponent *ComponentSystem::getSoundCompWithEId(int EID)
{
    for(auto temp : mSoundComponent)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

RenderComponent *ComponentSystem::getRenderCompWithEId(int EID)
{
    for(auto temp : mRenderComponents)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}

std::vector<RenderComponent *> ComponentSystem::getRenderComponents() const
{
    return mRenderComponents;
}

void ComponentSystem::addEmptyEntity(Entity *entity)
{
    mEntities.push_back(entity);
    numEntity++;
}

void ComponentSystem::addEntity(std::string filename, std::string material, std::string collision) // må adde flere ting som hvilke kollisjon osv.
{
    mEntities.push_back(new Entity(numEntity));
    mMeshComponents.push_back( mResourceFactory->makeResource(filename));
    mMaterialComponents.push_back(mResourceFactory->setMatComponent(mShaderProgram[0]));
    mTransformComponents.push_back(mResourceFactory->setTransComponent(gsl::Vector3D(0,0,0),0));
    mCollisionComponents.push_back(mResourceFactory->makeCollisionComponent(collision,numEntity));
    mRenderComponents.push_back(new RenderComponent(numEntity,getMeshCompWithEId(numEntity),getMatCompWithEId(numEntity),getTransCompWithEId(numEntity)));
    mMainWindow->addEntityInWorldContentList(numEntity);
    numEntity++;

}

std::vector<MeshComponent *> ComponentSystem::getMeshComponents() const
{
    return mMeshComponents;
}

std::vector<MaterialComponent *> ComponentSystem::getMaterialComponents() const
{
    return mMaterialComponents;
}

std::vector<TransformComponent *> ComponentSystem::getTransformComponents() const
{
    return mTransformComponents;
}

std::vector<CollisionComponent *> ComponentSystem::getCollisionComponents() const
{
    return mCollisionComponents;
}

std::vector<SoundComponent *> ComponentSystem::getSoundComponent() const
{
    return mSoundComponent;
}

std::vector<InputComponent *> ComponentSystem::getInputComponent() const
{
    return mInputComponent;
}

std::vector<Entity *> ComponentSystem::getEntities() const
{
    return mEntities;
}

Entity *ComponentSystem::getEntityWithEId(int EID)
{
    for(auto temp : mEntities)
    {
        if(static_cast<int>(temp->eID) == EID)
        {
            return temp;
        }
    }
    return nullptr;
}
