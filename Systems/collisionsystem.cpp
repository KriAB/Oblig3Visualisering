#include "collisionsystem.h"
#include "collisioncomponent.h"
#include "transformcomponent.h"
#include "componentsystem.h"

CollisionSystem::CollisionSystem(ComponentSystem *compSyst)
{
    compSys = compSyst;
}

bool CollisionSystem::collisionCheck(int EID, int otherEID)
{
    CollisionComponent *thisCol = compSys->getCollCompWithEId(EID); //finne i componentsystems
    CollisionComponent *otherCol = compSys->getCollCompWithEId(otherEID);

    TransformComponent *thisTrans = compSys->getTransCompWithEId(EID);
    TransformComponent *otherTrans = compSys->getTransCompWithEId(otherEID);

    if(thisCol != nullptr && otherCol != nullptr)
    {
        if(thisTrans == nullptr && otherTrans == nullptr)
        {
            std::cout << "Both entity " << EID <<" and "<< otherEID <<" does not have transform Components" << std::endl;
        }
        else
        {
            if(thisCol->shape == otherCol->shape)
            {
                if(thisCol->shape == CollisionShape::OBB)
                {
                    return collisionCheckOBB_OBB(thisCol,otherCol,otherTrans,thisTrans);
                }
                if(thisCol->shape == CollisionShape::AABB)
                {
                    return collisionCheckAABB_AABB(thisCol, otherCol,otherTrans,thisTrans);
                }
                if(thisCol->shape == CollisionShape::BS)
                {
                    return collisionCheckBS_Bs(thisCol,otherCol,otherTrans,thisTrans);
                }
            }
            else
            {
                if ((thisCol->shape == CollisionShape::BS) && (otherCol->shape == CollisionShape::OBB))
                {
                    return collisionCheckBS_OBB(thisCol, otherCol, otherTrans, thisTrans);
                }

                if ((thisCol->shape == CollisionShape::OBB) && (otherCol->shape == CollisionShape::BS))
                {
                    return collisionCheckBS_OBB(otherCol, thisCol ,thisTrans, otherTrans);
                }
            }

        }
    }
    return false;
}

bool CollisionSystem::collisionCheckAABB_AABB(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans)
{

    //Må rekalkulere hvis objektet har rotert seg
    //TODO: Må ha med om den er scala, bruke inverse matrisa

    float halfLengthX = thisCol->getLengthX()/2;
    float halfLengthY = thisCol->getLengthY()/2;
    float halfLengthZ = thisCol->getLengthZ()/2;

    float halfOtherX = otherCol->getLengthX()/2;
    float halfOtherY = otherCol->getLengthY()/2;
    float halfOtherZ = otherCol->getLengthZ()/2;

    //Kan sjekke om et av hjørnene er innenfor, må være true fra alle aksene. Sjekker først om den andre er mindre og innenfor første, så sjekker om den første er mindre og innenfor.
    //Regner feil her

    if((thisTrans->position().x - otherTrans->position().x < halfLengthX && thisTrans->position().x - otherTrans->position().x > -halfLengthX)
            || (thisTrans->position().x - otherTrans->position().x < halfOtherX && thisTrans->position().x - otherTrans->position().x > -halfOtherX))
        if((thisTrans->position().y - otherTrans->position().y < halfLengthY && thisTrans->position().y - otherTrans->position().y > -halfLengthY)
                || (thisTrans->position().y - otherTrans->position().y < halfOtherY && thisTrans->position().y - otherTrans->position().y > -halfOtherY))
            if((thisTrans->position().z - otherTrans->position().z < halfLengthZ && thisTrans->position().z - otherTrans->position().z > -halfLengthZ)
                    || (thisTrans->position().z - otherTrans->position().z < halfOtherZ && thisTrans->position().z - otherTrans->position().z > -halfOtherZ))
                return true;
    //No collision
    std::cout << "No collision between: Entity" << thisCol->eID << " and Entity" << otherCol->eID << std::endl;
    return false;

}

bool CollisionSystem::collisionCheckOBB_OBB(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans)
{
    return false;
}

bool CollisionSystem::collisionCheckBS_OBB(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans)
{
    float thisRadius = thisCol->radius();

    float halfOtherX = otherCol->getLengthX()/2;
    float halfOtherY = otherCol->getLengthY()/2;
    float halfOtherZ = otherCol->getLengthZ()/2;



    //TODO: sjekke Scalen til objektene og ta det med
    float lengthBetVec = thisTrans->position().lengthBetVec3d(otherTrans->position());
    if(lengthBetVec <= thisRadius )
        return true;
    else
    {

        if((thisTrans->position().x - otherTrans->position().x < halfOtherX && thisTrans->position().x - otherTrans->position().x > -halfOtherX))
            if((thisTrans->position().y - otherTrans->position().y < halfOtherY && thisTrans->position().y - otherTrans->position().y > -halfOtherY))
                if((thisTrans->position().z - otherTrans->position().z < halfOtherZ && thisTrans->position().z - otherTrans->position().z > -halfOtherZ))
                    return true;
    }

    return false;

}

bool CollisionSystem::collisionCheckBS_Bs(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans)
{
    float thisRadius = thisCol->radius();
    float otherRadius = otherCol->radius();

    //TODO: sjekke Scalen til objektene og ta det med
    float lengthBetVec = thisTrans->position().lengthBetVec3d(otherTrans->position());
    if(lengthBetVec <= thisRadius || lengthBetVec <= otherRadius)
        return true;
    return false;
}
