#include "collisioncomponent.h"
#include "transformcomponent.h"

CollisionComponent::CollisionComponent() : Components (compType::COLLISION)
{

}

bool CollisionComponent::collisionCheckAABB(CollisionComponent *other, TransformComponent *otherTrans, TransformComponent *thisTrans)
{
    //Må rekalkulere hvis objektet har rotert seg
    //Må ha med om den er scala, bruke inverse matrisa
    float xMin = smallestBiggestXYZ.at(0);
    float xMax = smallestBiggestXYZ.at(1);
    float yMin =smallestBiggestXYZ.at(2);
    float yMax = smallestBiggestXYZ.at(3);
    float zMin = smallestBiggestXYZ.at(4);
    float zMax = smallestBiggestXYZ.at(5);
    float xMinOther = other->smallestBiggestXYZ.at(0);
    float xMaxOther = other->smallestBiggestXYZ.at(1);
    float yMinOther = other->smallestBiggestXYZ.at(2);
    float yMaxOther = other->smallestBiggestXYZ.at(3);
    float zMinOther = other->smallestBiggestXYZ.at(4);
    float zMaxOther = other->smallestBiggestXYZ.at(5);

    lengthX = xMax - xMin; //osv
    lengthY = yMax - yMin;
    lengthZ = zMax - zMin;

    float lengthOtherx = xMaxOther - xMinOther;
    float lengthOthery = yMaxOther - yMinOther;
    float lengthOtherz = zMaxOther - zMinOther;

    float halfLengthX = lengthX/2;
     float halfLengthY = lengthY/2;
      float halfLengthZ = lengthZ/2;

      float halfOtherX = lengthOtherx/2;
      float halfOtherY = lengthOthery/2;
      float halfOtherZ = lengthOtherz/2;

    //Kan sjekke om et av hjørnene er innenfor, må være true fra alle aksene. Sjekker først om den andre er mindre og innenfor første, så sjekker om den første er mindre og innenfor.
      //Regner feil her

      if((thisTrans->position().x - otherTrans->position().x < halfLengthX && thisTrans->position().x - otherTrans->position().x > -halfLengthX)
              || (thisTrans->position().x - otherTrans->position().x < halfOtherX && thisTrans->position().x - otherTrans->position().x > -halfOtherX))
          if((thisTrans->position().y - otherTrans->position().y < halfLengthY && thisTrans->position().y - otherTrans->position().y > -halfLengthY)
                  || (thisTrans->position().y - otherTrans->position().y < halfOtherY && thisTrans->position().y - otherTrans->position().y > -halfOtherY))
          if((thisTrans->position().z - otherTrans->position().z < halfLengthZ && thisTrans->position().z - otherTrans->position().z > -halfLengthZ)
                  || (thisTrans->position().z - otherTrans->position().z < halfOtherZ && thisTrans->position().z - otherTrans->position().z > -halfOtherZ))
                {
                    //collision
                 //   std::cout << "Collision between: Entity" << this->eID << " and Entity" << other->eID << std::endl;
                    return true;
          }


        //No collision
        //std::cout << "No collision X!" << std::endl;
        return false;

}

std::vector<Vertex> CollisionComponent::vertices() const
{
    return mVertices;
}


float CollisionComponent::getLengthX()
{
    float xMin = smallestBiggestXYZ.at(0);
    float xMax = smallestBiggestXYZ.at(1);
    return lengthX = xMax - xMin;
}

float CollisionComponent::getLengthY()
{
    float yMin =smallestBiggestXYZ.at(2);
    float yMax = smallestBiggestXYZ.at(3);
    return lengthY = yMax - yMin;
}
float CollisionComponent::getLengthZ()
{
    float zMin = smallestBiggestXYZ.at(4);
    float zMax = smallestBiggestXYZ.at(5);
    return lengthZ = zMax - zMin;
}

float CollisionComponent::radius()
{
    float radius{0};
    getLengthX();
    getLengthY();
    getLengthZ();
    if(lengthX > lengthY && lengthX > lengthZ)
    {
        radius = lengthX/2;
    }
    else if(lengthY > lengthX && lengthY > lengthZ)
    {
        radius = lengthY/2;
    }
    else
        radius = lengthZ/2;

    return radius;

}


std::vector<int> CollisionComponent::indices() const
{
    return mIndices;
}

void CollisionComponent::setIndices(const std::vector<int> &indices)
{
    mIndices = indices;
}
