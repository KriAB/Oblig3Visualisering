#include "componentsystem.h"
#include "resourcefactory.h"
#include "Scenes/scene.h"
#include "renderwindow.h"
#include "mainwindow.h"
#include "shader.h"
#include "Physics/gravity.h"

ComponentSystem::ComponentSystem(RenderWindow *renderwindow, MainWindow *mainWindow, Shader * shaderProgram) : mRenderWindow(renderwindow), mMainWindow(mainWindow),   mShaderProgram{shaderProgram}
{
    mResourceFactory = new ResourceFactory;
    mResourceFactory->mRenderWindow = mRenderWindow;
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


    mEntities.push_back(new Entity(0));
    mMeshComponents.push_back(mResourceFactory->makeResource("Plane2"));
    mMaterialComponents.push_back(mResourceFactory->setMatComponent(mShaderProgram[0]));
    mTransformComponents.push_back(mResourceFactory->setTransComponent(gsl::Vector3D(0,0,0),0));
    // mCollisionComponents.push_back(mResourceFactory->makeCollisionComponent("OBB",0));
    mRenderComponents.push_back(new RenderComponent(0,mMeshComponents.at(0),mMaterialComponents.at(0),mTransformComponents.at(0)));
    mMainWindow->addEntityInWorldContentList(0);
    initGravity(0);
    numEntity++;

    //Item 1 i forhold til b-spline
    addEntity("Cube", "Default", "OBB", gsl::Vector3D{2,2,2});
    updateHeightAndBarycentricCheck(1); // setter høyden riktig i forhold til planet, så den ikke svever.
    //mResourceFactory->addChild(0,1);

    //Item 2 i forhold til b-spline
    addEntity("Cube", "Default", "OBB", gsl::Vector3D{20,2,26});
    updateHeightAndBarycentricCheck(2);

    //Item 3 i forhold til b-spline
    addEntity("Cube", "Default", "OBB", gsl::Vector3D{4,2,26});
    updateHeightAndBarycentricCheck(3);

    //Item 4 i forhold til b-spline
    addEntity("Cube", "Default", "OBB", gsl::Vector3D{20,2,4});
    updateHeightAndBarycentricCheck(4);

    //Player
    addEntity("Ball", "Default", "BS", gsl::Vector3D{4,2,8});
    updateHeightAndBarycentricCheck(5);
    mInputComponent.push_back(new InputComponent(5,InputMovementTypes::MOVE_UP, InputMovementTypes::MOVE_DOWN, InputMovementTypes::MOVE_LEFT,InputMovementTypes::MOVE_RIGHT, InputFireTypes::NO_FIRE));
}

void ComponentSystem::move(int EID, gsl::Vector3D translate)
{
   TransformComponent* trans = getTransCompWithEId(EID);
   if(trans != nullptr)
   {
       trans->translate(translate);
   }
   else
   {
       std::cout << "No TransformComponent with Eid: " << EID << std::endl;
   }
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

InputComponent *ComponentSystem::getInputCompWithEId(int EID)
{
    for(auto temp : mInputComponent)
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

void ComponentSystem::addEntity(std::string filename, std::string material, std::string collision, gsl::Vector3D position) // må adde flere ting som hvilke kollisjon osv.
{
    mEntities.push_back(new Entity(numEntity));
    mMeshComponents.push_back( mResourceFactory->makeResource(filename));
    mMaterialComponents.push_back(mResourceFactory->setMatComponent(mShaderProgram[0]));
    mTransformComponents.push_back(mResourceFactory->setTransComponent(position,0));
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

//Her setter man hvilket objekt (plan) som feks. "kulen" skal sjekke om den er innenfor. Må settes før man updater gravity og height
void ComponentSystem::initGravity(int EID)
{
    mGravity = new Gravity;
    mGravity->setTriangles(getMeshCompWithEId(EID)->vertices(),getMeshCompWithEId(EID)->neighbours());
    CollisionComponent *temp = getCollCompWithEId(EID);
    if(temp != nullptr)
    {
        temp->hasGravity = true;
    }
    else
    {
        //Bør adde collision?
    }

}
//Her oppdaterer man høyden til objektet i forhold til planet som har initGravity.
void ComponentSystem::updateHeightAndBarycentricCheck(int EIDTarget)
{

    if( mGravity->update(getTransCompWithEId(EIDTarget)->position()))
    {
        float heigth = mGravity->getHeight();

    //    std::cout << "Cube " <<EIDTarget << " height: " << getTransCompWithEId(EIDTarget)->matrix().getPosition().y << "Calculated height: " << heigth << std::endl;
        getTransCompWithEId(EIDTarget)->matrix().setHeightY(heigth+getMeshCompWithEId(EIDTarget)->radiusY());

    }

}
void ComponentSystem::initNPC(int EID, std::vector<Entity*> items)
{
    //initialiserer NPC på en valgt Entity, sender en vector med entites som skal være items
}

void ComponentSystem::updateNPC(int EID)
{
    // oppdaterer NPCen slik at den kjører state maskinen
    //Her må man kjøre en move, som

}

//Her oppdaterer man "kulen". Denne funksjonen er ikke for en player. Kun for å simulere en kule som ruller på et ujevnt plan.
void ComponentSystem::updateVelocity(int EIDTarget, float deltaTime)
{


    if(getCollCompWithEId(EIDTarget)->hasGravity)
    {
        // mGravity->setTarget(getTransCompWithEId(EIDTarget)->position(), getMeshCompWithEId(EIDTarget)->neighbours());

        //mGravity->setRadius(float) fra mesh, for å sette en annen enn 1.2
        mGravity->update(getTransCompWithEId(EIDTarget)->position());

        gsl::Vector3D acceleration = mGravity->getAcceleration();
        if(mGravity->isOnTriangle == true)
        {

            getTransCompWithEId(EIDTarget)->setVelocity(getTransCompWithEId(EIDTarget)->getVelocity() + acceleration*deltaTime);
            getTransCompWithEId(EIDTarget)->translate(getTransCompWithEId(EIDTarget)->getVelocity());
            updateHeight(EIDTarget);
        }
        else
        {

            getTransCompWithEId(EIDTarget)->matrix().setPosition(getTransCompWithEId(EIDTarget)->getStartPosition().x,
                                                                 getTransCompWithEId(EIDTarget)->getStartPosition().y,
                                                                 getTransCompWithEId(EIDTarget)->getStartPosition().z);
            getTransCompWithEId(EIDTarget)->setVelocity(gsl::Vector3D(0,0,0));
        }

    }
}
//This is for use with updateVelocity(...);
void ComponentSystem::updateHeight(int EIDTarget)
{
    if(getCollCompWithEId(EIDTarget)->hasGravity)
    {
        float heigth = mGravity->getHeight();
        getTransCompWithEId(EIDTarget)->matrix().setHeightY(heigth + getMeshCompWithEId(EIDTarget)->radiusY());
        // std::cout << "Ball height: " << transformComponents.at(EID)->matrix().getPosition().y << "Calculated height: " << heigth << std::endl;

    }
}
