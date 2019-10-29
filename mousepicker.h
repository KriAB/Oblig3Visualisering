#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H
#include "innpch.h"
#include "camera.h"
class QMouseEvent;
class RenderWindow;
class ResourceFactory;
class SystemManager;

class MousePicker
{
public:
    MousePicker(RenderWindow *renderWindow, Camera *cam, SystemManager *syst);
    gsl::Vector3D getCurrentRay(){return currentRay;}
    void update();
    gsl::Vector3D getIntersectionPoint();
    gsl::Vector3D getPointOnRay(gsl::Vector3D ray, float distance);
    int intersectedEntity();

private:

    gsl::Vector3D currentRay;
    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;
    Camera *currentCamera;

    gsl::Vector3D calculateMouseRay();
    gsl::Vector2D getNormalizedDeviceCoordinates(float xMouse, float yMouse);
    gsl::Vector4D toEyeCoords(gsl::Vector4D clipCoords);
    gsl::Vector3D toWorldCoords(gsl::Vector4D eyeCoords);
    RenderWindow *mRenderWindow;

    float rayLenght = 20;

    gsl::Vector3D intersectionPoint;



    SystemManager *mSystemManager;

};

#endif // MOUSEPICKER_H
