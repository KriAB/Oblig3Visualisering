#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <chrono>
#include "texture.h"
#include "camera.h"
#include "visualobject.h"
#include "input.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class Light;
class ResourceFactory;
class MeshComponent;
class MaterialComponent;
class TransformComponent;
class CollisionComponent;
class Entity;
class MousePicker;

class SystemManager;
/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;
    void toggleWireframe();

    void checkForGLerrors();

    std::vector<std::string> returnCurrentObjectNames();
    VisualObject *ReturnObj(int num){
        if(mVisualObjects.size() > num)return mVisualObjects.at(num); else {return nullptr;} }

    gsl::Vector3D getCameraPosition(){ return mCurrentCamera->position();}
    gsl::Vector3D getCameraForward(){return mCurrentCamera->forward();}
    gsl::Vector3D getCameraUp(){return mCurrentCamera->up();}
    float getCameraSpeed(){return mCurrentCamera->speed();}

    //disse sendes inn til mainwindow
    //transform
    //material
    //collision
    MeshComponent* getMeshComponent(unsigned int EID);
    TransformComponent* getTransComponent(unsigned int EID);
    MaterialComponent* getMaterialComponent(unsigned int EID);
       CollisionComponent* getCollisionComponent(unsigned int EID);
       std::vector<Entity*> getEntities();
       Entity *getEntity(unsigned int EID);


       int mMouseX{0};
       int mMouseY{0};

       QSize getWindowSize();
       SystemManager *getSystemManager() const;

       Input &getInput();

private slots:
       void render();

private:
    void init();
    void setCameraSpeed(float value);

    QOpenGLContext *mContext{nullptr};
    bool mInitialized{false};

    Texture *mTexture[4]{nullptr}; //We can hold 4 textures
    Shader *mShaderProgram[4]{nullptr}; //We can hold 4 shaders

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform0{-1};
    GLint vMatrixUniform0{-1};
    GLint pMatrixUniform0{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    std::vector<VisualObject*> mVisualObjects;

    ResourceFactory *mFactory;

    VisualObject *mPlayer;  //the controllable object
    Light *mLight;

    Camera *mCurrentCamera{nullptr};

    bool mWireframe{false};

    Input mInput;
    float mCameraSpeed{0.01f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};


    QTimer *mRenderTimer{nullptr};  //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    float mAspectratio{1.f};

    MainWindow *mMainWindow{nullptr};    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void calculateFramerate();

    void startOpenGLDebugger();

    void handleInput();

    std::chrono::high_resolution_clock::time_point mLastTime;

protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    bool hasBeenInit =false;

    int numBall = 0;
    int numCube = 0;
    int numPlane = 0;

    int moveParent = 0;
    int moveChild = 0;
    float speed = 0.001;

    bool soundOn = true;

    int pickedEntity{-1};
    MousePicker *picker;
    int previousPick = -1;

   std::vector<gsl::Vector3D> startPos;

   SystemManager *mSystemManager;

};

#endif // RENDERWINDOW_H
