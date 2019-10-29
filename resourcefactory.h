#ifndef RESOURCEFACTORY_H
#define RESOURCEFACTORY_H
#include "material.h"
#include "renderwindow.h"
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "shader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "soundcomponent.h"
#include <cstdint>    //does not work well on mac
//#include <stdint.h>     //using this on Mac instead of cstdint. not a totally clean solution
#include <string>
#include <cstdio>
#include "Entity.h"
#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

class MeshComponent;
class TransformComponent;
class MaterialComponent;
class SoundComponent;
class CollisionComponent;
class ResourceFactory : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceFactory();
    ~ResourceFactory();


   // resourceFactory *instance;

    void init();
    void initCollision();
    void initOneObject();
    void draw();
    void readObjFile(std::string filename);
    void readTxtFile(std::string filename);
//    bool readWaveFile(std::string filename/*, wave_t* wavePtr*/);
//    bool endOnError(std::string errmsg);

    void addChild(int aEID, int bEID);
    void move(int mEID, gsl::Vector3D translate);


   // gsl::Matrix4x4 mMatrix;

    //void setShader(Shader *shader);


    RenderWindow *mRenderWindow; //Just to be able to call checkForGLerrors()


    //Material mMaterial;
    MeshComponent *makeResource(std::string fileName);
    void setMeshComponent();
    TransformComponent* setTransComponent(gsl::Vector3D translate, float scale);
    MaterialComponent *setMatComponent(Shader *shader);
    void makeSoundComponent(unsigned int mEID);
    void activateSound(unsigned int mEID);
    void stopSound();
    void cleanUpSound();
    void updateSoundListener();
    void getCameraInfo();
    CollisionComponent *makeCollisionComponent(std::string type, int EID);
    std::vector<Vertex> collisionShape(std::string type);


    std::vector<MeshComponent *> getMeshComponents() ;

    std::vector<TransformComponent *> getTransformComponents() ;

    std::vector<MaterialComponent *> getMaterialComponents();

    std::vector<CollisionComponent *> getCollisionComponents() ;



    std::vector<Entity *> getEntities() const;

private:

    std::vector<Vertex> makeBS(float radius);
    std::map<std::string, int> meshMap; //finne eID i meshComponents

    MeshComponent * meshComp = nullptr;
    std::vector <MeshComponent*> meshComponents;

    TransformComponent *transComp = nullptr;
    std::vector<TransformComponent*> transformComponents;

    MaterialComponent * matComp = nullptr;
    std::vector<MaterialComponent*> materialComponents;

    CollisionComponent *collComp =nullptr;
    std::vector<CollisionComponent*> collisionComponents;


    Entity *mEntity = nullptr;
    std::vector<Entity*> entities;
    unsigned int eID =0;

    gsl::Vector3D cameraPosition{0,0,0};
    gsl::Vector3D cameraForward{0,0,-1};
    gsl::Vector3D cameraUp{0,1,0};
    float cameraSpeed{0};


    //For collision
   void setSmallestBiggestXYZ();
protected:
    void makeSkybox();
    void makeXYZ();
    void makePlane();

    std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mIndices;    //This is usually not needed after object is made

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)


    //For collision
     std::array <float,6> smallestBiggestXYZ;


    //For sound

     SoundSource* mStereoSound{};

    /// Plays the sound source from the current position.
    void play();
    /// Pauses the sound source at the current position.
    void pause();
    /// Stops the sound source and rewinds to start.
    void stop();

    /// Loads one given WAVE file.
    /**
        Calls the wave loader from the FileHandler class, parses the wave data and buffers it.
        \param The file path relative to execution directory.
    **/
    bool loadWave(std::string filePath);

    void setPosition(gsl::Vector3D newPos);             ///< Sets source position from Vector3.
    gsl::Vector3D getPosition() {return mPosition;}     ///< Returns source position as Vector3.
    void setVelocity(gsl::Vector3D newVel);             ///< Sets source velocity from Vector3.
    gsl::Vector3D getVelocity() {return mVelocity;}     ///< Returns source velocity as Vector3.

private:
    std::string mName;          ///< The name of the sound source (Not used).
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;             ///< The data buffer.
    gsl::Vector3D mPosition;    ///< Vector containing source position.
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    bool checkError(std::string name);
};

#endif // RESOURCEFACTORY_H
