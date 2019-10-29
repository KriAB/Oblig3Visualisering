#include "mousepicker.h"
#include <QMouseEvent>
#include "renderwindow.h"
#include "resourcefactory.h"
#include "transformcomponent.h"
#include "meshcomponent.h"
#include "collisioncomponent.h"
#include "systemmanager.h"
#include "componentsystem.h"
MousePicker::MousePicker(RenderWindow *renderWindow, Camera *cam,  SystemManager *syst/*, gsl::Matrix4x4 projMat*/)
{
    mRenderWindow = renderWindow;
    currentCamera = cam;
    mProjectionMatrix = currentCamera->mProjectionMatrix;
    mViewMatrix = currentCamera->mViewMatrix;
    mSystemManager = syst;
}

void MousePicker::update()
{
    mViewMatrix = currentCamera->mViewMatrix;
    mProjectionMatrix = currentCamera->mProjectionMatrix;
    currentRay = calculateMouseRay();

}

gsl::Vector3D MousePicker::calculateMouseRay()
{
    //Mouse er i Viewport space, må convertere til worldspace
    float mouseX =mRenderWindow->mMouseX; //putte inn i renderWindow, så det kan henter derfra heller
    float mouseY = mRenderWindow->mMouseY;
    //Convert from Viewport space to Normalized device space
    gsl::Vector2D normalizedCoords = getNormalizedDeviceCoordinates(mouseX,mouseY);

   // std::cout << normalizedCoords << std::endl;

    //Convert from Normalized device space to homogeneous clip space
    gsl::Vector4D clipCoords = gsl::Vector4D(normalizedCoords.x,normalizedCoords.y, -1.f,1.f);
     //  std::cout << clipCoords << std::endl;
    //Convert from Clip space to Eye space
    gsl::Vector4D eyeCoords = toEyeCoords(clipCoords);
      //std::cout << eyeCoords << std::endl;
    //Convert from Eye space to World space
    gsl::Vector3D worldRay = toWorldCoords(eyeCoords);
     //std::cout << worldRay << std::endl;
   // std::cout << "World Ray: " << worldRay <<std::endl;
    return worldRay;

}

gsl::Vector2D MousePicker::getNormalizedDeviceCoordinates(float xMouse, float yMouse)
{
    QSize windowSize = mRenderWindow->getWindowSize();
    float x = (2.0f*xMouse) / windowSize.width() -1;
    float y = 1 - (2.0f*yMouse) / windowSize.height();
    return gsl::Vector2D(x,y); //kan være jeg må returnere (x, -y)
}

gsl::Vector4D MousePicker::toEyeCoords(gsl::Vector4D clipCoords)
{
    gsl::Matrix4x4 invertedProjection = mProjectionMatrix;


    invertedProjection.inverse();

    gsl::Vector4D eyeCoords = invertedProjection*clipCoords;
    //Kun interessert i x,y
    eyeCoords = gsl::Vector4D(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    return eyeCoords;
}

gsl::Vector3D MousePicker::toWorldCoords(gsl::Vector4D eyeCoords)
{
    gsl::Matrix4x4 invertetViewMat = mViewMatrix;
    invertetViewMat.inverse();
    gsl::Vector4D rayWorld = invertetViewMat * eyeCoords;
    gsl::Vector3D rayMouse = gsl::Vector3D(rayWorld.x,rayWorld.y,rayWorld.z);
    rayMouse.normalize();
    return  rayMouse;

}

gsl::Vector3D MousePicker::getIntersectionPoint()
{

    return intersectionPoint;
}

gsl::Vector3D MousePicker::getPointOnRay(gsl::Vector3D ray, float distance)
{
    gsl::Vector3D camPos = currentCamera->position();
    gsl::Vector3D start = gsl::Vector3D(camPos.x,camPos.y,camPos.z);
    gsl::Vector3D scaledRay = gsl::Vector3D(ray.x*distance, ray.y*rayLenght, ray.z*distance);
    gsl::Vector3D pointOnRay = start+scaledRay;

    return  pointOnRay;
}

int MousePicker::intersectedEntity()
{
    std::vector<TransformComponent*>tempTrans = mSystemManager->componentSystem()->getTransformComponents();
    std::vector<MeshComponent*> tempMeshes = mSystemManager->componentSystem()->getMeshComponents();
    std::vector<CollisionComponent*> tempColl = mSystemManager->componentSystem()->getCollisionComponents();

    gsl::Vector3D tempIntersectionCoord = currentRay;

    //Hvis man får lengden mellom posisjonen til objektet og rayen
 //   std::cout << "IntersectionPoint: " << tempIntersectionCoord <<std::endl;

    //sjekke at Objektet har kollisjon, hvis ikke bruke meshen. Må tenke på scale
    for(int i = 0; i <tempTrans.size(); i++)
    {
        //Loop som sjekker for forskjellige lengder til rayen
        for(float j = 0; j < rayLenght; j ++)
        {
            tempIntersectionCoord = currentRay;

            tempIntersectionCoord = getPointOnRay(currentRay,j);

            if(tempColl.at(i) != nullptr)
            {
//                if(tempTrans.at(i)->position().x - tempIntersectionCoord.x < tempColl.at(i)->getLengthX()/2 && tempTrans.at(i)->position().x - tempIntersectionCoord.x > -tempColl.at(i)->getLengthX()/2 )
//                    if(tempTrans.at(i)->position().y - tempIntersectionCoord.y < tempColl.at(i)->getLengthY()/2 && tempTrans.at(i)->position().y - tempIntersectionCoord.y > -tempColl.at(i)->getLengthY()/2 )
//                        if(tempTrans.at(i)->position().z - tempIntersectionCoord.z < tempColl.at(i)->getLengthZ()/2 && tempTrans.at(i)->position().z - tempIntersectionCoord.z > -tempColl.at(i)->getLengthZ()/2 )
//                        {
//                            intersectionPoint = tempIntersectionCoord;
//                          std::cout << "IntersectionPoint: " << intersectionPoint << " Entity: " << i << std::endl;
//                            return i;
//                        }

                //Må være true for alle x,y,z
                if(tempTrans.at(i)->position().x - ((tempColl.at(i)->getLengthX())/2) < tempIntersectionCoord.x && tempTrans.at(i)->position().x + tempColl.at(i)->getLengthX()/2 > tempIntersectionCoord.x)
                {
                    if(tempTrans.at(i)->position().y - ((tempColl.at(i)->getLengthY())/2) < tempIntersectionCoord.y && tempTrans.at(i)->position().y + tempColl.at(i)->getLengthY()/2 > tempIntersectionCoord.x)
                    {
                        if(tempTrans.at(i)->position().z - ((tempColl.at(i)->getLengthZ())/2) < tempIntersectionCoord.z && tempTrans.at(i)->position().z + tempColl.at(i)->getLengthZ()/2 > tempIntersectionCoord.z)
                        {
                            intersectionPoint = tempIntersectionCoord;
                          std::cout << "IntersectionPoint: " << intersectionPoint << " Entity: " << i << std::endl;
                            return i;
                        }
                    }
                }   //Hvis ikke bruk meshens minste og største verdi. Må finne en bedre løsning
                else if(tempMeshes.at(i) != nullptr)
                {

                    if(tempTrans.at(i)->position().x+tempMeshes.at(i)->getSmallestBiggestXYZ().at(0) < tempIntersectionCoord.x && tempTrans.at(i)->position().x+tempMeshes.at(i)->getSmallestBiggestXYZ().at(1) > tempIntersectionCoord.x)
                    {
                        if(tempTrans.at(i)->position().y+tempMeshes.at(i)->getSmallestBiggestXYZ().at(2) < tempIntersectionCoord.y && tempTrans.at(i)->position().y+tempMeshes.at(i)->getSmallestBiggestXYZ().at(3) > tempIntersectionCoord.y)
                        {
                            if(tempTrans.at(i)->position().z+tempMeshes.at(i)->getSmallestBiggestXYZ().at(4) < tempIntersectionCoord.z && tempTrans.at(i)->position().z+tempMeshes.at(i)->getSmallestBiggestXYZ().at(5) > tempIntersectionCoord.z)
                            {
                                intersectionPoint = tempIntersectionCoord;
                                   //std::cout << "IntersectionPoint: " << intersectionPoint << " Entity: " << i << " Position: " << tempTrans.at(i)->position()<<std::endl;
                                return i;
                            }
                        }
                    }


//                    if(tempTrans.at(i)->position().x - tempIntersectionCoord.x < tempMeshes.at(i)->getLengthX()/2 && tempTrans.at(i)->position().x - tempIntersectionCoord.x > -tempMeshes.at(i)->getLengthX()/2 )
//                        if(tempTrans.at(i)->position().y - tempIntersectionCoord.y < tempMeshes.at(i)->getLengthY()/2 && tempTrans.at(i)->position().y - tempIntersectionCoord.y > -tempMeshes.at(i)->getLengthY()/2 )
//                            if(tempTrans.at(i)->position().z - tempIntersectionCoord.z < tempMeshes.at(i)->getLengthZ()/2 && tempTrans.at(i)->position().z - tempIntersectionCoord.z > -tempMeshes.at(i)->getLengthZ()/2 )
//                            {
//                                intersectionPoint = tempIntersectionCoord;
//                               std::cout << "IntersectionPoint: " << intersectionPoint << " Entity: " << i << std::endl;
//                                return i;
//                            }


                }
                else
                {
                    std::cout << "Something wrong with Intersected Entity!" << std::endl;
                    return -1;
                }
            }
        }

    }
return - 1;
}
