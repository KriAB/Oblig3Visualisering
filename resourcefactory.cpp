#include "resourcefactory.h"
#include "innpch.h"
//#include <fstream>
//#include <sstream>
#include <iostream>
#include "meshcomponent.h"
#include "transformcomponent.h"
#include "materialcomponent.h"
#include "soundcomponent.h"
#include "collisioncomponent.h"
#include "octahedronball.h"
#include "soundmanager.h"
#include "soundsource.h"
#include "Physics/gravity.h"
#include "Triangulation/regular.h"
#include "lasloader.h"


#include <chrono>   //for sleep_for
#include <thread>   //for sleep_for



ResourceFactory::ResourceFactory()
{

}

MeshComponent* ResourceFactory::makeResource(std::string fileName)
{
    //Sette alt tilbake til 0, så objektet ikke får verdier fra et tidligere objekt.
    mVertices.clear();
    mIndices.clear();
    neighbours.clear();
    mVAO = 0;
    mVBO = 0;
    mEAB = 0;

    mEntity = new Entity();
    meshComp = new MeshComponent();
    meshComp->eID = eID;
    meshComp->name = fileName;

    transComp = new TransformComponent();
    transComp->eID = eID;
    transformComponents.push_back(transComp);

    matComp = new MaterialComponent();
    matComp->eID = eID;
    materialComponents.push_back(matComp);

    mEntity->eID = eID;
    entities.push_back(mEntity);
    auto count = meshMap.find(fileName);
    if(count != meshMap.end())
    {
        //fant fileName

        int temp  =  meshMap[fileName];

        for(int i = 0; i < meshComponents.size(); i++)
        {
            if(meshComponents.at(i)->eID == temp)
            {

                meshComp->setVAO(meshComponents.at(i)->vAO());
                meshComp->setEAB(meshComponents.at(i)->eAB());
                meshComp->indicesSize = meshComponents.at(i)->indicesSize;
                meshComp->verticesSize = meshComponents.at(i)->verticesSize;
                meshComp->setNeighbours(meshComponents.at(i)->neighbours());
                meshComp->setSmalestBiggestXYZ(meshComponents.at(i)->getSmallestBiggestXYZ());
                meshComp->eID = eID;
                meshComponents.push_back(meshComp);

                std::cout << "In map: " << fileName << std::endl;
            }
        }

    } //meshComp = meshComponents.at(0);
    //hvis meshen finnes, kopier dataen fra den meshen. på eId plassen, må endre eId til ny eId

    else
    {
        //putte inn meshen i map og lage den

        meshMap.insert(std::pair<std::string, int>(fileName, eID));
        meshMap.emplace(fileName,eID);
        std::cout << "Not in map: " << fileName << " eID: " << eID << std::endl;

        if(fileName.find(".obj") != std::string::npos)
        {
            readObjFile(fileName);
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();
        }

        if(fileName.find(".txt") != std::string::npos)
        {
            readTxtFile(fileName);
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();
        }

        if(fileName == "Skybox")
        {
            makeSkybox();
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();
        }

        if(fileName == "Plane")
        {
            makePlane();
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();

        }
        if(fileName == "Cube")
        {
            readTxtFile("box2.txt");
            setMeshComponent();

            meshComponents.push_back(meshComp);
            initOneObject();
        }
        if(fileName == "Ball")
        {
            OctahedronBall *ball;
            ball = new OctahedronBall(2);
            mIndices =  ball->returnIndices();
            mVertices = ball->returnVertices();
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();
        }
        if(fileName == "XYZ")
        {
            makeXYZ();
            setMeshComponent();
            meshComp->drawTriangles = false;
            meshComponents.push_back(meshComp);
            initOneObject();
        }
        //Oblig 2 Del 1 - oppgave 1
        if(fileName == "Plane2")
        {
            readTxtFileWithNeighbours("planeWithNeighbours.txt");
            //makePlane2();
            setMeshComponent();
            meshComponents.push_back(meshComp);
            initOneObject();
        }
        if(fileName == "LAS")
        {
           // readTxtFileWithNeighbours("32LasFileVertices20.txt");

          //  readTxtFileWithNeighbours("32LasFileVertices20NB.txt");

            readTxtFileWithNeighbours("32LasFileVertices20FlippedNB.txt");
            //makeLAS(fileName);
            setMeshComponent();
             //meshComp->drawTriangles = false;
            meshComponents.push_back(meshComp);
            initOneObject();
        }

        setSmallestBiggestXYZ();
        meshComp->setSmalestBiggestXYZ(smallestBiggestXYZ);

    }


    std::cout << "Size of meshComponents: " << meshComponents.size() << "Material size: "<< std::endl;
    eID++;

    return meshComp;
}

//Oblig 2
void ResourceFactory::makePlane2()
{
    float height1 = 0.5; //4 points
    float height2 = 2; //3 points
    float height3 = 2; //1 point
    float height4 = 0.f; //6 points
    float height5 = 0.5; // 4 points
    float height6 = 2; // 2 points
    float height7 = 2;// 3 points
    float height8 = 2; // 1 point
    float lenght =30;
    Vertex v{};
    //T0
    v.set_xyz(0,height1,lenght/2); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height2,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0,height3,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{-1,-1,1}); //starter på 1
    //T1
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height2,0); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0,height1,lenght/2); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{0,6,2}); //starter på 1

    //T2
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(lenght,height5,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height2,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{-1,1,3}); //starter på 1

    //T3
    v.set_xyz(lenght,height6,lenght/2); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(lenght,height5,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{2,4,-1}); //starter på 1

    //T4
    v.set_xyz(lenght,height5,lenght); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(lenght,height6,lenght/2); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{3,5,-1}); //starter på 1

    //T5
    v.set_xyz(lenght,height5,lenght); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height7,lenght); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{6,-1,5}); //starter på 1

    //T6
    v.set_xyz(lenght/2,height7,lenght); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height4,lenght/2); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0,height1,lenght/2); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{1,7,5}); //starter på 1

    //T7
    v.set_xyz(0,height8,lenght); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(lenght/2,height7,lenght); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0,height1,lenght/2); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    neighbours.push_back(gsl::Vector3D{6,-1,-1}); //starter på 1

  //  writeTxtFileNeighbours("planeWithNeighbours.txt");

}

void ResourceFactory::makeLAS(std::string filename)
{
    Regular reg;

//     gsl::LASLoader lasloader{"../INNgine2019/Assets/Meshes/32-1-498-104-61.las"};
//    // Iterate in certain range:
//     std::vector<gsl::Vector3D> vecFromLAS;

//    int numOfPoints = lasloader.pointCount();
//    std::cout << "NumOfPoints "<<numOfPoints << std::endl;
//    vecFromLAS.reserve(numOfPoints-600000);
//     //Del 2 - oppg 2:
//     //Konverter høydedata fra valgt fil til eget filformat(xyz linjevis)
//    for (auto point = lasloader.begin() + 0; point != lasloader.end() - 600000; ++point)
//    {
//        gsl::Vector3D temp(point->x,point->z, point->y);
//        vecFromLAS.push_back(temp);
//    }
//    //writeTxtFileVector3("LASmap1.txt", vecFromLAS);



//    //Del 2 - oppg 3:
//    //Lag en triangulering.
//    reg.setLASCoords(vecFromLAS);
//    reg.makeTriangles(10,numOfPoints);
//    std::vector <gsl::Vector3D> triangleTemp = reg.getFinalTriangles();

//    for(int i = 0; i < triangleTemp.size(); i++)
//    {
//        gsl::Vector3D norm =triangleTemp.at(i);
//        norm.normalize();
//                mVertices.push_back(Vertex(triangleTemp[i].x, triangleTemp[i].y, triangleTemp[i].z,norm.x,norm.y,norm.z));
//    }

    //    for(int i = 0; i < vecFromLAS.size(); i++)
    //    {
    //        gsl::Vector3D norm =vecFromLAS.at(i);
    //        norm.normalize();
    //              //  mVertices.push_back(Vertex(vecFromLAS[i].x, vecFromLAS[i].y, vecFromLAS[i].z,norm.x,norm.y,norm.z));
    //                  mVertices.push_back(Vertex(norm.x, norm.y, norm.z,norm.x,norm.y,norm.z));
    //    }

    // Iterate in certain range with normalized output:
//    for (auto point = lasloader.begin() + 500; point != lasloader.end() - 25; ++point)
//    {
//        std::cout << "Point: (" << point->xNorm() << ", " << point->yNorm() << ", " << point->zNorm() << ")" << std::endl;
//        mVertices.push_back(Vertex(point->xNorm(),point->yNorm(), point->zNorm(),  1,1,0));
//    }

//----- Read and write LAS to txt file
 //    writeLASToTxtFileVector3("32LasFileFlipped");
// ----- Sort file
    std::vector<gsl::Vector3D> temp = readTxtFileVec3("32LasFileNormFlippedEvery20.txt");
     std::vector<gsl::Vector3D> tempTemp;
     tempTemp.reserve(temp.size());
    for(int i = 0; i <temp.size() ; i++)
    {
        tempTemp.push_back(temp.at(i));
    }
    reg.setLASCoords(tempTemp);
//   // reg.sort();
////// ----- Make triangles
    reg.makeTriangles(10,temp.size());
    int sizeOfTri = reg.triangleSize();
    std::vector<gsl::Vector3D> triangles;
    triangles.reserve(sizeOfTri);
    triangles = reg.getFinalTriangles();

    mVertices.empty();
    mVertices.resize(0);
    for(auto tri :triangles)
    {
        mVertices.push_back(Vertex(tri.x*100, tri.y*100 -70,tri.z*100, tri.x,tri.y,tri.z));
    }

    neighbours.reserve(reg.neighbourSize());
    neighbours = reg.getNeighbours();

writeTxtFileNeighbours("32LasFileVertices20FlippedNB.txt");


}

void ResourceFactory::setMeshComponent()
{
    meshComp->setVertices(mVertices);
    meshComp->setIndices(mIndices);
    meshComp->setNeighbours(neighbours);
}

TransformComponent *ResourceFactory::setTransComponent(gsl::Vector3D translate, float scale)
{
    // transformComponents.back()->translate(translate);
    transComp->translate(translate);
    if(scale >0.01 || scale <-0.01)
    {
        transComp->scale(scale);
        //  transformComponents.back()->scale(scale);
    }
    return transComp;
}

MaterialComponent *ResourceFactory::setMatComponent(Shader *shader)
{
    matComp->setShader(shader);
    // materialComponents.back()->setShader(shader);
    return matComp;
}

void ResourceFactory::makeSoundComponent(unsigned int mEID)
{
    // SoundSource* mStereoSound{};

    SoundManager::getInstance()->init();

    gsl::Vector3D componentPosition{0,0,0};
    //For å finne possisjonen til objektet componenten tilhører
    for(auto comp : transformComponents)
    {
        if(mEID == comp->eID)
            componentPosition = comp->position();
    }
    mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", componentPosition,
                "../INNgine2019/Assets/Sounds/Caravan_mono.wav", true, 1.0f); //false for ikke loop

}

void ResourceFactory::activateSound(unsigned int mEID)
{
    getCameraInfo();
    //plays the sounds
    mStereoSound->play();
    gsl::Vector3D componentPosition{0,0,0};
    //For å finne possisjonen til objektet componenten tilhører
    for(auto comp : transformComponents)
    {
        if(mEID == comp->eID)
            componentPosition = comp->position();
    }


    mStereoSound->setPosition(gsl::Vector3D(componentPosition));
    //    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    //set the listener position
    //  SoundManager::getInstance()->updateListener(cameraPosition, cameraSpeed, cameraForward, cameraUp);


    //    //vector for source placement
    //      gsl::Vector3D sourceVec3;

    //      //Must cleanly shut down the soundmanager
    //      SoundManager::getInstance()->cleanUp();
}

void ResourceFactory::stopSound()
{
    mStereoSound->stop();
}

void ResourceFactory::cleanUpSound()
{

    //      //Must cleanly shut down the soundmanager
    SoundManager::getInstance()->cleanUp(); //TODO fikse not working cleanUp
}

void ResourceFactory::updateSoundListener()
{
    getCameraInfo();

    SoundManager::getInstance()->updateListener(cameraPosition, cameraSpeed, cameraForward, cameraUp);
}

void ResourceFactory::getCameraInfo()
{
    cameraSpeed = mRenderWindow->getCameraSpeed();
    cameraForward = mRenderWindow->getCameraForward();
    cameraPosition = mRenderWindow->getCameraPosition();
    cameraUp = mRenderWindow->getCameraUp();
}

CollisionComponent* ResourceFactory::makeCollisionComponent(std::string type, int EID)
{
    //bør sjekke om Enitien har komponenten fra før
    if(EID == -1)
    {
        collComp = new CollisionComponent;
        collComp->setVertices(collisionShape(type));
        initCollision();
        collComp->eID = this->eID-1;
        collisionComponents.push_back(collComp);
    }

    else
    {
        collComp = new CollisionComponent;
        collComp->setVertices(collisionShape(type));
        initCollision();
        collComp->eID = EID;
        collisionComponents.push_back(collComp);
    }
return collComp;
}

std::vector<Vertex> ResourceFactory::collisionShape(std::string type)
{
    std::vector<Vertex> temp;
    std::vector<int> mIndices;
    float radius;
    float lenghtX = smallestBiggestXYZ.at(1)-smallestBiggestXYZ.at(0);
    float lenghtY = smallestBiggestXYZ.at(3)-smallestBiggestXYZ.at(2);
    float lenghtZ = smallestBiggestXYZ.at(5)-smallestBiggestXYZ.at(4);

    collComp->smallestBiggestXYZ =smallestBiggestXYZ;

    if(type == "OBB") //Dette er egentlig AABB
    {
        collComp->shape = CollisionShape::OBB;

        temp.push_back(Vertex{smallestBiggestXYZ.at(0), smallestBiggestXYZ.at(2),smallestBiggestXYZ.at(4),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(0), smallestBiggestXYZ.at(3),smallestBiggestXYZ.at(4),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(1), smallestBiggestXYZ.at(3),smallestBiggestXYZ.at(4),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(1), smallestBiggestXYZ.at(2),smallestBiggestXYZ.at(4),1.f, 1.f, 0.1f});

        temp.push_back(Vertex{smallestBiggestXYZ.at(0), smallestBiggestXYZ.at(2),smallestBiggestXYZ.at(5),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(0), smallestBiggestXYZ.at(3),smallestBiggestXYZ.at(5),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(1), smallestBiggestXYZ.at(3),smallestBiggestXYZ.at(5),1.f, 1.f, 0.1f});
        temp.push_back(Vertex{smallestBiggestXYZ.at(1), smallestBiggestXYZ.at(2),smallestBiggestXYZ.at(5),1.f, 1.f, 0.1f});



    }
    if(type ==  "BS")
    {
        collComp->shape = CollisionShape::BS;
        //finne radius
        if(lenghtX >=lenghtY && lenghtX >= lenghtZ)
            radius = (lenghtX/2);

        else if(lenghtY>=lenghtX && lenghtY >=lenghtZ)
            radius =(lenghtY/2);

        else
            radius =(lenghtZ/2);

        //Lage vertices for sphere
       temp = makeBS(radius);
    }    if(type == "AABB")
    {
        collComp->shape = CollisionShape::AABB;

    }
    return temp;
}

std::vector<MeshComponent *> ResourceFactory::getMeshComponents()
{
    return meshComponents;
}

std::vector<TransformComponent *> ResourceFactory::getTransformComponents()
{
    return transformComponents;
}

std::vector<MaterialComponent *> ResourceFactory::getMaterialComponents()
{
    return materialComponents;
}

std::vector<CollisionComponent *> ResourceFactory::getCollisionComponents()
{
    return collisionComponents;
}

std::vector<Entity *> ResourceFactory::getEntities() const
{
    return entities;
}

std::vector<Vertex> ResourceFactory::makeBS(float radius)
{
    std::vector<Vertex> temp;
    float oneThird = radius/3;
    //#1
    temp.push_back(Vertex{oneThird, -radius,0, 1.f, 1.f, 0.1f});
    temp.push_back(Vertex{-oneThird, -radius,0, 1.f, 1.f, 0.1f});
    //#2
//    temp.push_back(Vertex{-oneThird, -radius,0, 1.f, 1.f, 0.1f});
//    temp.push_back(Vertex{-radius, -oneThird,0, 1.f, 1.f, 0.1f});
    //#3
    temp.push_back(Vertex{-radius, -oneThird,0, 1.f, 1.f, 0.1f});
    temp.push_back(Vertex{-radius, oneThird,0, 1.f, 1.f, 0.1f});
    //#4
//     temp.push_back(Vertex{-radius, oneThird,0, 1.f, 1.f, 0.1f});
//     temp.push_back(Vertex{-oneThird, radius,0, 1.f, 1.f, 0.1f});
    //#5
     temp.push_back(Vertex{-radius, radius,0, 1.f, 1.f, 0.1f});
     temp.push_back(Vertex{oneThird, radius,0, 1.f, 1.f, 0.1f});
    //#6
//     temp.push_back(Vertex{oneThird, radius,0, 1.f, 1.f, 0.1f});
//     temp.push_back(Vertex{radius,  oneThird,0, 1.f, 1.f, 0.1f});
    //#7
     temp.push_back(Vertex{radius,  oneThird,0, 1.f, 1.f, 0.1f});
     temp.push_back(Vertex{radius, -oneThird,0, 1.f, 1.f, 0.1f});
    //#8
//     temp.push_back(Vertex{radius, -oneThird,0, 1.f, 1.f, 0.1f});
//     temp.push_back(Vertex{oneThird, -radius,0, 1.f, 1.f, 0.1f});

     //#1
     temp.push_back(Vertex{0, -radius,oneThird, 1.f, 1.f, 0.1f});
     temp.push_back(Vertex{0, -radius,-oneThird, 1.f, 1.f, 0.1f});
     //#2
//     temp.push_back(Vertex{0, -radius,-oneThird, 1.f, 1.f, 0.1f});
//     temp.push_back(Vertex{0, -oneThird,-radius, 1.f, 1.f, 0.1f});
     //#3
     temp.push_back(Vertex{0, -oneThird,-radius, 1.f, 1.f, 0.1f});
     temp.push_back(Vertex{0, oneThird,-radius, 1.f, 1.f, 0.1f});
     //#4
//      temp.push_back(Vertex{0, oneThird,-radius, 1.f, 1.f, 0.1f});
//      temp.push_back(Vertex{0, radius,-oneThird, 1.f, 1.f, 0.1f});
     //#5
      temp.push_back(Vertex{0, radius,-radius, 1.f, 1.f, 0.1f});
      temp.push_back(Vertex{0, radius,oneThird, 1.f, 1.f, 0.1f});
     //#6
//      temp.push_back(Vertex{0, radius,oneThird, 1.f, 1.f, 0.1f});
//      temp.push_back(Vertex{0,  oneThird,radius, 1.f, 1.f, 0.1f});
     //#7
      temp.push_back(Vertex{0,  oneThird,radius, 1.f, 1.f, 0.1f});
      temp.push_back(Vertex{0, -oneThird,radius, 1.f, 1.f, 0.1f});
     //#8
//      temp.push_back(Vertex{0, -oneThird,radius, 1.f, 1.f, 0.1f});
//      temp.push_back(Vertex{0, -radius,oneThird, 1.f, 1.f, 0.1f});



    return temp;
}

void ResourceFactory::setSmallestBiggestXYZ()
{
    float xBiggest = mVertices.at(0).get_XYZ().x;
    float xSmallest = mVertices.at(0).get_XYZ().x;
    float yBiggest = mVertices.at(0).get_XYZ().y;
    float ySmallest = mVertices.at(0).get_XYZ().y;
    float zBiggest = mVertices.at(0).get_XYZ().z;
    float zSmallest = mVertices.at(0).get_XYZ().z;
    //Cheking for smallest/biggest
    for (auto v : mVertices)
    {   //for x
        if(v.get_XYZ().x > xBiggest)
        {
            xBiggest = v.get_XYZ().x;
        }

        if(v.get_XYZ().x < xSmallest)
        {
            xSmallest = v.get_XYZ().x;
        }
        //For y
        if(v.get_XYZ().y > yBiggest)
        {
            yBiggest = v.get_XYZ().y;
        }

        if(v.get_XYZ().y < ySmallest)
        {
            ySmallest = v.get_XYZ().y;
        }
        // For z
        if(v.get_XYZ().z > zBiggest)
        {
            zBiggest = v.get_XYZ().z;
        }

        if(v.get_XYZ().z < zSmallest)
        {
            zSmallest = v.get_XYZ().z;
        }
    }


    smallestBiggestXYZ.at(0) = xSmallest;
    smallestBiggestXYZ.at(1) = xBiggest;
    smallestBiggestXYZ.at(2) = ySmallest;
    smallestBiggestXYZ.at(3) = yBiggest;
    smallestBiggestXYZ.at(4) = zSmallest;
    smallestBiggestXYZ.at(5) = zBiggest;
    //  collComp->smallestBiggestXYZ = smallestBiggestXYZ;
}



void ResourceFactory::makeSkybox()
{
    mVertices.insert( mVertices.end(),
    {//Vertex data for front
     Vertex{gsl::Vector3D(-1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.333f)},  //v0
     Vertex{gsl::Vector3D( 1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.333f)},  //v1
     Vertex{gsl::Vector3D(-1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.666f)},    //v2
     Vertex{gsl::Vector3D( 1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.666f)},    //v3

     //Vertex data for right
     Vertex{gsl::Vector3D(1.f, -1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.333f)},  //v4
     Vertex{gsl::Vector3D(1.f, -1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.333f)},  //v5
     Vertex{gsl::Vector3D(1.f,  1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},    //v6
     Vertex{gsl::Vector3D(1.f,  1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.666f)},    //v7

     //Vertex data for back
     Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.333f)},  //v8
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.333f)},  //v9
     Vertex{gsl::Vector3D( 1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.666f)},    //v10
     Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.666f)},    //v11

     //Vertex data for left
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.333f)},    //v12
     Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},  //v13
     Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.666f)},      //v14
     Vertex{gsl::Vector3D(-1.f,  1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f,   0.666f)},  //v15

     //Vertex data for bottom
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.f)},      //v16
     Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.f)},      //v17
     Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},   //v18
     Vertex{gsl::Vector3D( 1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.333f)},   //v19

     //Vertex data for top
     Vertex{gsl::Vector3D(-1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.666f)},    //v20
     Vertex{gsl::Vector3D( 1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},    //v21
     Vertex{gsl::Vector3D(-1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.999f)},      //v22
     Vertex{gsl::Vector3D( 1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.999f)}       //v23
                      });

    mIndices.insert( mIndices.end(),
    { 0,  2,  1,  1,  2,  3,      //Face 0 - triangle strip (v0,  v1,  v2,  v3)
      4,  6,  5,  5,  6,  7,      //Face 1 - triangle strip (v4,  v5,  v6,  v7)
      8,  10,  9, 9, 10, 11,      //Face 2 - triangle strip (v8,  v9, v10,  v11)
      12, 14, 13, 13, 14, 15,     //Face 3 - triangle strip (v12, v13, v14, v15)
      16, 18, 17, 17, 18, 19,     //Face 4 - triangle strip (v16, v17, v18, v19)
      20, 22, 21, 21, 22, 23      //Face 5 - triangle strip (v20, v21, v22, v23)
                     });
}

void ResourceFactory::makeXYZ()
{
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

}

void ResourceFactory::makePlane()
{
    Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);

}
ResourceFactory::~ResourceFactory()
{

    //    //      //Must cleanly shut down the soundmanager
    //    SoundManager::getInstance()->cleanUp(); //Puttet inn i destructor i renderwindow

    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );


    delete meshComp;
    meshComp = nullptr;

    delete transComp;
    transComp = nullptr;

    delete matComp;
    matComp = nullptr;

    delete collComp;
    collComp =nullptr;

    delete mEntity;
    mEntity = nullptr;

    delete mRenderWindow;
    mRenderWindow = nullptr;

    delete mGravity;
    mGravity = nullptr;

}
void ResourceFactory::init()
{
    initializeOpenGLFunctions();

    for(auto comps: meshComponents)
    {
        //must call this to use OpenGL functions

        //Vertex Array Object - VAO
        glGenVertexArrays( 1, &comps->vAO());// mVAO );
        glBindVertexArray( comps->vAO() );
        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers( 1, &comps->vBO() );
        glBindBuffer( GL_ARRAY_BUFFER, comps->vBO());

        glBufferData( GL_ARRAY_BUFFER, comps->vertices().size()*sizeof(Vertex), comps->vertices().data(), GL_STATIC_DRAW );
        // 1rst attribute buffer : vertices
        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // 2nd attribute buffer : colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(1);

        // 3rd attribute buffer : uvs
        glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
        glEnableVertexAttribArray(2);

        if(comps->indices().size() != 0)
        {
            //Second buffer - holds the indices (Element Array Buffer - EAB):
            glGenBuffers(1, &comps->eAB());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comps->eAB());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, comps->indices().size() * sizeof(GLuint), comps->indices().data(), GL_STATIC_DRAW);
        }
        glBindVertexArray(0);
    }
}

void ResourceFactory::initCollision()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &collComp->vAO());// mVAO );
    glBindVertexArray( collComp->vAO() );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &collComp->vBO() );
    glBindBuffer( GL_ARRAY_BUFFER, collComp->vBO());

    glBufferData( GL_ARRAY_BUFFER, collComp->verticesSize*sizeof(Vertex), collComp->vertices().data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ResourceFactory::initOneObject()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &meshComp->vAO());// mVAO );
    glBindVertexArray( meshComp->vAO() );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &meshComp->vBO() );
    glBindBuffer( GL_ARRAY_BUFFER, meshComp->vBO());

    glBufferData( GL_ARRAY_BUFFER, meshComp->verticesSize*sizeof(Vertex), meshComp->vertices().data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);


    if(meshComp->indices().size() != 0)
    {
        //Second buffer - holds the indices (Element Array Buffer - EAB):
        glGenBuffers(1, &meshComp->eAB());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp->eAB());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComp->indices().size() * sizeof(GLuint), meshComp->indices().data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

void ResourceFactory::draw()
{

    //Antar at mesh, trans og mat er like store
    for(int i = 0; i< meshComponents.size(); i++)
    {
        glUseProgram(materialComponents.at(i)->returnMaterial().mShader->getProgram());
        glBindVertexArray( meshComponents.at(i)->vAO() );
        materialComponents.at(i)->returnMaterial().mShader->transmitUniformData(&transformComponents.at(i)->matrix(), &materialComponents.at(i)->returnMaterial());
        if(meshComponents[i]->indicesSize== 0)
        {
            if(meshComponents[i]->drawTriangles == true)
                glDrawArrays(GL_TRIANGLES, 0, meshComponents.at(i)->verticesSize);
            else
            {
                glDrawArrays(GL_LINES, 0, meshComponents.at(i)->verticesSize);
            }
        }
        else
        {
            glDrawElements(GL_TRIANGLES, meshComponents.at(i)->indicesSize, GL_UNSIGNED_INT, nullptr);
        }
    }
    //TODO fix this
    for(int i = 0; i < collisionComponents.size();i++)
    {
        if(collisionComponents.at(i)->isRenderingBox)
        {
            glUseProgram(materialComponents.at(0)->returnMaterial().mShader->getProgram());
            glBindVertexArray( collisionComponents.at(i)->vAO() );
            materialComponents.at(0)->returnMaterial().mShader->transmitUniformData(&transformComponents.at(i)->matrix());
            glDrawArrays(GL_LINE_LOOP, 0, collisionComponents.at(i)->verticesSize);
        }
    }

}

//void resourceFactory::setShader(Shader *shader)
//{
//    mMaterial.mShader = shader;
//}


void ResourceFactory::readObjFile(std::string filename)
{
    //Open File
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;
    std::ifstream fileIn;
    fileIn.open (fileWithPath, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertecies;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    //    std::vector<Vertex> mVertices;    //made in VisualObject
    //    std::vector<GLushort> mIndices;   //made in VisualObject

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }

            //For some reason the winding order is backwards so fixing this by swapping the last two indices
            //Update: this was because the matrix library was wrong - now it is corrected so this is no longer needed.
            //            unsigned int back = mIndices.size() - 1;
            //            std::swap(mIndices.at(back), mIndices.at(back-1));
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();
    qDebug() << "Obj file read: " << QString::fromStdString(filename);
}

void ResourceFactory::readTxtFile(std::string filename) {
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {   //Hvis filen ikke er i Assets/Meshes/, så sjekk VisAssets/
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
        inn.close();

        fileWithPath = gsl::projectFolderName + "VisAssets/" + filename;
        inn.open(fileWithPath);

        if (inn.is_open()) {
            int n;
            Vertex vertex;
            inn >> n;
            mVertices.reserve(n);
            for (int i=0; i<n; i++) {
                inn >> vertex;
                mVertices.push_back(vertex);
            }
            inn.close();
            qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
        }
        else
        {
            qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
        }
    }
}

//Del 2 - Oppgave 2
void ResourceFactory::writeLASToTxtFileVector3(std::string filename)
{
    filename = "32LasFileNormFlippedEvery20.txt";
    std::ofstream ut;
    ut.open(filename.c_str());

    Regular reg;

     gsl::LASLoader lasloader{"../INNgine2019/Assets/Meshes/32-1-498-104-61.las"};
    // Iterate in certain range:

    if (ut.is_open())
    {
        auto n =lasloader.pointCount()/20;
        gsl::Vector3D vec3;
        ut << n << std::endl;
        for (auto it=lasloader.begin(); it != lasloader.end(); it=it+19)
        {
              ut << it->xNorm() << ' ' << it->zNorm()  << ' ' << it->yNorm() <<std::endl;
        }
        ut.close();
    }
}

void ResourceFactory::readTxtFileWithNeighbours(std::string filename)
{
    std::ifstream inn;
    std::string fileWithPath = gsl::projectFolderName + "VisAssets/" + filename;

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int numberOfVertices;
        int numberOfNeighbours;
        Vertex vertex;
        gsl::Vector3D neigh;
        inn >> numberOfVertices;
        inn >> numberOfNeighbours;
        mVertices.reserve(numberOfVertices);
        neighbours.reserve(numberOfNeighbours);
        for (int i=0; i<numberOfVertices; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        for (int i=0; i<numberOfNeighbours; i++) {
            inn >> neigh;
            neighbours.push_back(neigh);
        }

        inn.close();
        qDebug() << "Factory file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }

}

std::vector<gsl::Vector3D> ResourceFactory::readTxtFileVec3(std::string filename)
{
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    inn.open(fileWithPath);

  std::vector <gsl::Vector3D> vec3;
    if (inn.is_open()) {
        int n;
        gsl::Vector3D vec;
        inn >> n;
        vec3.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vec;
            vec3.push_back(vec);
        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
    return vec3;
}

void ResourceFactory::writeTxtFileNeighbours(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {
        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;

        gsl::Vector3D neigh;
        n = neighbours.size();
        ut << n << std::endl;

        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex << std::endl;
        }

        for(auto it = neighbours.begin(); it != neighbours.end(); it++)
        {
            neigh = *it;
            ut << neigh << std::endl;
        }
        ut.close();
    }
}

void ResourceFactory::move(int mEID, gsl::Vector3D translate, bool rotate, gsl::Vector3D rotAxis, GLfloat deg)
{
    for(auto trans: transformComponents)
    {
        if(trans->eID == mEID)
        {
            trans->translate(translate);
            Entity *temp = entities.at(mEID)->getChild();
            if(temp != nullptr)
            {
                transformComponents.at(temp->eID)->translate(translate);
            }
        }

        if(rotate == true)
        {
            if(rotAxis.x > 0.01f || rotAxis.x < 0.01f)
            {
                trans->rotateX(deg);
            }
            if(rotAxis.y > 0.01f || rotAxis.y < 0.01f)
            {
                trans->rotateY(deg);
            }
            if(rotAxis.z > 0.01f || rotAxis.z < 0.01f)
            {
                trans->rotateZ(deg);
            }
        }
    }
}

void ResourceFactory::addChild(int aEID, int bEID)
{
    //makeResource(filename);

    if(aEID < entities.size())
    {
        if(aEID != bEID)
            entities.at(aEID)->setChild(entities.at(bEID));
    }

}

void ResourceFactory::move(int mEID, gsl::Vector3D translate)
{
    for(auto trans: transformComponents)
    {
        if(trans->eID == mEID)
        {
            trans->translate(translate);
            Entity *temp = entities.at(mEID)->getChild();
            if(temp != nullptr)
            {
                transformComponents.at(temp->eID)->translate(translate);
            }
        }
    }
}

void ResourceFactory::initGravity()
{
    mGravity = new Gravity;
    mGravity->setTriangles(mVertices,neighbours);


}

void ResourceFactory::updateGravity(int EIDTarget, float deltaTime)
{

//    mGravity->setTarget(transformComponents.at(EIDTarget)->position(), );

//    mGravity->update(transformComponents.at(EIDTarget)->position());

//    gsl::Vector3D acceleration = mGravity->getAcceleration();
//    if(mGravity->isOnTriangle == true)
//    {

//        transformComponents.at(EIDTarget)->setVelocity(transformComponents.at(EIDTarget)->getVelocity() + acceleration*deltaTime);
//        transformComponents.at(EIDTarget)->translate(transformComponents.at(EIDTarget)->getVelocity());
//    }
//    else
//    {

//        transformComponents.at(EIDTarget)->matrix().setPosition(transformComponents.at(EIDTarget)->getStartPosition().x,
//                                                                transformComponents.at(EIDTarget)->getStartPosition().y,
//                                                                transformComponents.at(EIDTarget)->getStartPosition().z);
//        transformComponents.at(EIDTarget)->setVelocity(gsl::Vector3D(0,0,0));
//    }
}

void ResourceFactory::setHeightAfterTriangles(int EID)
{

    float heigth = mGravity->getHeight();
    transformComponents.at(EID)->matrix().setHeightY(heigth);
    // std::cout << "Ball height: " << transformComponents.at(EID)->matrix().getPosition().y << "Calculated height: " << heigth << std::endl;

}

