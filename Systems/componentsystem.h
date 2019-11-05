#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H
#include "materialcomponent.h"
#include "meshcomponent.h"
#include "rendercomponent.h"
#include "transformcomponent.h"
#include "collisioncomponent.h"
#include "soundcomponent.h"
#include "inputcomponent.h"
#include "Entity.h"

#include <QOpenGLFunctions_4_1_Core>

class ResourceFactory;
class Scene;
class RenderWindow;
class MainWindow;
class Shader;
class Gravity;
class SystemManager;

class ComponentSystem : public QOpenGLFunctions_4_1_Core
{
public:
    ComponentSystem(RenderWindow *renderwindow, MainWindow *mainWindow, Shader *shaderProgram, SystemManager *mSystemManager ); //We can hold 4 shaders);
    ~ComponentSystem();
    // arrays til MeshComponent
    //arrays til MaterialComponent
    //GameObject *mGameObject;

    //mGameObject->addComponent(mMeshComponent);
    //sjekker om objectet har childs/siblings, Må gi objektene lik traslasjon osv.

    void sortByEID();
//    void init();
//   void update();

    void saveScene(std::string name);
    void loadScene(std::string name);
    void makeScene();
    void move(int EID, gsl::Vector3D translate);


    Entity *getEntityWithEId(int EID);

    TransformComponent *getTransCompWithEId(int EID);
    MeshComponent *getMeshCompWithEId(int EID);
    CollisionComponent *getCollCompWithEId(int EID);
    MaterialComponent *getMatCompWithEId(int EID);
    SoundComponent *getSoundCompWithEId(int EID);
    InputComponent *getInputCompWithEId(int EID);
    RenderComponent *getRenderCompWithEId(int EID);



    void addTransComp(TransformComponent *transComp);
    void addMeshComp(MeshComponent *meshComp);//...
    void addEmptyEntity(Entity *entity);
    void addEntity(std::string filename, std::string material, std::string collision, gsl::Vector3D position= gsl::Vector3D{0,0,0});

    std::vector<Entity *> getEntities() const;
    std::vector<MeshComponent *> getMeshComponents() const;
    std::vector<MaterialComponent *> getMaterialComponents() const;
    std::vector<TransformComponent *> getTransformComponents() const;
    std::vector<CollisionComponent *> getCollisionComponents() const;
    std::vector<SoundComponent *> getSoundComponent() const;
    std::vector<InputComponent *> getInputComponent() const;
    std::vector<RenderComponent *> getRenderComponents() const;

    //For calc behavior on a terrain.
    void initGravity(int EID);
    void updateVelocity(int EIDTarget, float deltaTime);
    void updateHeight(int EIDTarget);
    void updateHeightAndBarycentricCheck(int EIDTarget);
private:
    std::vector <MeshComponent*> mMeshComponents; //sortere på Entity ID //til renderSystem
    std::vector <MaterialComponent*> mMaterialComponents; //til renderSystem
    std::vector <RenderComponent*> mRenderComponents;
    std::vector <TransformComponent*> mTransformComponents;
    std::vector <CollisionComponent*> mCollisionComponents;
    std::vector <SoundComponent*> mSoundComponent;
    std::vector <InputComponent*> mInputComponent;

    std::vector <Entity*> mEntities; //conteiner med alle objektene. Kan eks. brukes i gui for å se og velge objekter.

    ResourceFactory *mResourceFactory{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    MainWindow *mMainWindow{nullptr};
    Shader *mShaderProgram[4]{nullptr};
    SystemManager * mSystemManager{nullptr};

    int numEntity{0};

    Scene *mScene{nullptr};


    //For Barycentric:
    Gravity *mGravity = nullptr;
    std::vector<gsl::Vector3D> neighbours;



    //For Oblig 3
    void initNPC(int EID, std::vector<Entity *> items);
    void updateNPC(int EID);

    //For å lagre items
    std::vector<TransformComponent*> items;
};

#endif // COMPONENTSYSTEM_H
