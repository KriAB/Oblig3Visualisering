#ifndef SCENE_H
#define SCENE_H
#include "Components/materialcomponent.h"
#include "Components/meshcomponent.h"
#include <vector>
class Entity;
class Camera;
class ComponentSystem;
class  QJsonObject;
class Camera;

class Scene
{
public:
    Scene(ComponentSystem *compSys);
    bool openSceneFromFile(std::string filename);
    void saveSceneInfile(std::string filename);
    void read(QJsonArray data);

    bool writeScene(std::string filename);
    //    // arrays til MeshComponent
    //    //arrays til MaterialComponent
    //    //GameObject *mGameObject;

    //    //mGameObject->addComponent(mMeshComponent);
    //    //sjekker om objectet har childs/siblings, Må gi objektene lik traslasjon osv.

    //private:
    //    std::vector <MeshComponent> mMeshComponents;
    //    std::vector <MaterialComponent> mMaterialComponent;

    //    std::vector <GameObject*> gameObjects; //conteiner med alle objektene. Kan eks. brukes i gui for å se og velge objekter.

    //scenegraf scenehåntering
    //scenetreet holder Entities, og kanskje ta det i forhold til possisjon
    //kan ha de som har kollisjon i samme gren og så sortere etter kordinater.

    void setGameCamera(Camera *gameCamera){mGameCamera = gameCamera;}

private:

    Camera *mGameCamera;

    QJsonObject writeEntityToFile(int EID);
    QJsonObject writeTrans(int EID);
    QJsonObject writeInput(int EID);
    QJsonObject writeMesh(int EID);
    QJsonObject writeMaterial(int EID);
    QJsonObject writeColl(int EID);
    QJsonObject writeSound(int EID);
    QJsonObject writeRender(int EID);
    QJsonObject writePhysics(int EID);
    QJsonObject writeLight(int EID);
    QJsonObject writeScript(int EID);
    QJsonObject writeCamera(int EID);
    QJsonObject writeMotion(int EID);

    void readTrans(int EID, QJsonObject &data);
    void readInput(int EID, QJsonObject &data);
    void readMesh(int EID, QJsonObject &data);
    void readMat(int EID, QJsonObject &data);
    void readColl(int EID, QJsonObject &data);
    void readSound(int EID, QJsonObject &data);
    void readRender(int EID, QJsonObject &data);
    void readPhysics(int EID, QJsonObject &data);
    void readLight(int EID, QJsonObject &data);
    void readScript(int EID, QJsonObject &data);
    void readCamera(int EID, QJsonObject &data);
    void readMotion(int EID, QJsonObject &data);

    ComponentSystem *mComponentSystem;
};

#endif // SCENE_H
