#include "scene.h"
#include "camera.h"
#include "Entity.h"
#include "componentsystem.h"
#include "camera.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>


Scene::Scene(ComponentSystem *compSys)
{
    mComponentSystem = compSys;
}

bool Scene::openSceneFromFile(std::string filename)
{
    // her må json filen leses
    QFile openFile(QString::fromStdString(filename));

    QJsonArray enityArray;

    if(!openFile.open(QIODevice::ReadOnly))
    {
     qWarning("Couldn't opene saved file.");
     return false;
    }
    QByteArray data = openFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromBinaryData(data));

    read(loadDoc.array());

    return true;
}

void Scene::saveSceneInfile(std::string filename)
{

}
// https://doc-snapshots.qt.io/qt5-5.9/qtcore-json-savegame-example.html
void Scene::read(QJsonArray data)
{
QJsonObject temp;
int eId;
    // read all info from the array
    for(int i = 0; i < data.size(); i++)
    {
        temp = data.at(i).toObject();
        eId =  temp["EntityID"].toInt();
    }

}

bool Scene::writeScene(std::string filename)
{
  QFile saveFile(QStringLiteral("level1.json"));

      if (!saveFile.open(QIODevice::WriteOnly)) {
          qWarning("Couldn't open save file.");
          return false;
      }
      QJsonArray entityArray;

      for(int i = 0; i < static_cast<int>(mComponentSystem->getEntities().size()); i++)
      {
         entityArray.insert(i, writeEntityToFile(i));
      }

      QJsonDocument saveDoc(entityArray);

      saveFile.write(saveDoc.toJson());

      return true;

}

QJsonObject Scene::writeEntityToFile(int EID)
{
    QJsonObject entity;
    entity["EntityID"] = EID;
    if(mComponentSystem->getEntityWithEId(EID)->getChild() == nullptr)
    {
        entity["Child"] = -1;
    }
    else
    {
        entity["Child"] = QString(mComponentSystem->getEntityWithEId(EID)->getChild()->eID);
    }
    if(mComponentSystem->getEntityWithEId(EID)->getSibling() == nullptr)
    {
        entity["Sibling"] = -1;
    }
    else
    {
        entity["Sibling"] = QString(mComponentSystem->getEntityWithEId(EID)->getSibling()->eID);
    }

    QJsonObject transComp = writeTrans(EID);
    QJsonObject meshComp = writeMesh(EID);
    QJsonObject matComp = writeMaterial(EID);
    QJsonObject renderComp = writeRender(EID);
    QJsonObject inputComp = writeInput(EID);
    QJsonObject collComp = writeColl(EID);
    QJsonObject soundComp = writeSound(EID);

    QJsonObject lightComp = writeColl(EID);
    QJsonObject scriptComp = writeSound(EID);
    QJsonObject motionComp = writeMotion(EID);
    QJsonObject camComp = writeCamera(EID);

    entity["TransformComponent"] = transComp;
    entity["MeshComponent"] = meshComp;
    entity["MaterialComponent"] = matComp;
    entity["RenderComponent"] = renderComp;
    entity["InputComponent"] = inputComp;
    entity["CollisionComponent"] = collComp;
    entity["SoundComponent"] = soundComp;
    entity["LightComponent"] = lightComp;
    entity["ScriptComponent"] = scriptComp;
    entity["MotionComponent"] = motionComp;
    entity["CameraComponent"] = camComp;

    return entity;
}

QJsonObject Scene::writeTrans(int EID)
{
    QJsonObject transComp;
    if(mComponentSystem->getTransCompWithEId(EID) != nullptr)
    {
        transComp["hasComp"] =true;
        QJsonArray posAr = {
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().x),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().y),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().z)
        };
        QJsonArray rotAr = {
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().x),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().y),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().z)
        };
        QJsonArray scaleAr = {
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().x),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().y),
            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().z)
        };
        transComp["Position"] = posAr;
        transComp["Rotation"] = rotAr;
        transComp["Scale"] = scaleAr;
    }
    else
    {
        transComp["hasComp"] = false;
    }
    return transComp;
}

QJsonObject Scene::writeInput(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

QJsonObject Scene::writeMesh(int EID)
{
    QJsonObject meshComp;

    if(mComponentSystem->getMeshCompWithEId(EID) != nullptr)
    {
        meshComp["hasComp"] = true;
        meshComp["Name"] = QString::fromStdString(mComponentSystem->getMeshCompWithEId(EID)->name);
        meshComp["IsDrawingTriangles"] = mComponentSystem->getMeshCompWithEId(EID)->drawTriangles;
    }
    else
    {
        meshComp["hasComp"] = false;
    }
    return meshComp;
}

QJsonObject Scene::writeMaterial(int EID)
{
    QJsonObject matComp;
    if(mComponentSystem->getMatCompWithEId(EID) != nullptr)
    {
        matComp["hasComp"] = true;
    }
    else
    {
        matComp["hasComp"] = false;
    }
    return  matComp;
}

QJsonObject Scene::writeColl(int EID)
{
    QJsonObject comp;
    if(mComponentSystem->getCollCompWithEId(EID) != nullptr)
    {
        comp["hasComp"] = true;
    }
    else
    {
        comp["hasComp"] = false;
    }
    return  comp;
}

QJsonObject Scene::writeSound(int EID)
{
    QJsonObject comp;
    if(mComponentSystem->getSoundCompWithEId(EID) != nullptr)
    {
        comp["hasComp"] = true;
    }
    else
    {
        comp["hasComp"] = false;
    }
    return  comp;
}

QJsonObject Scene::writeRender(int EID)
{

    QJsonObject comp;
    if(mComponentSystem->getRenderCompWithEId(EID) != nullptr)
    {
        comp["hasComp"] = true;
    }
    else
    {
        comp["hasComp"] = false;
    }
    return  comp;
}

QJsonObject Scene::writePhysics(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

QJsonObject Scene::writeLight(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

QJsonObject Scene::writeScript(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

QJsonObject Scene::writeCamera(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

QJsonObject Scene::writeMotion(int EID)
{
    QJsonObject comp;

    comp["hasComp"] = false;

    return  comp;
}

void Scene::readTrans(int EID, QJsonObject &data)
{
//    QJsonObject transComp;
//    if(mComponentSystem->getTransCompWithEId(EID) != nullptr)
//    {
//        transComp["hasComp"] =true;
//        QJsonArray posAr = {
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().x),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().y),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->position().z)
//        };
//        QJsonArray rotAr = {
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().x),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().y),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getRotation().z)
//        };
//        QJsonArray scaleAr = {
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().x),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().y),
//            static_cast<double>(mComponentSystem->getTransCompWithEId(EID)->getScale().z)
//        };
//        transComp["Position"] = posAr;
//        transComp["Rotation"] = rotAr;
//        transComp["Scale"] = scaleAr;
//    }
//    else
//    {
//        transComp["hasComp"] = false;
//    }


}

