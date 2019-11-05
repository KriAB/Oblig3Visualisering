#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
class CollisionComponent;
class TransformComponent;
class ComponentSystem;

class CollisionSystem
{
public:
    //Collision detection system
    CollisionSystem(ComponentSystem *compSyst);
    bool collisionCheck(int EID, int otherEID);

private:
    bool collisionCheckAABB_AABB(CollisionComponent* thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);
    bool collisionCheckBS_Bs(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);

    bool collisionCheckAABB_BS(CollisionComponent* thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);
    bool collisionCheckBS_AABB(CollisionComponent *thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);

    //Trengs ikke for spillet
    bool collisionCheckOBB_OBB(CollisionComponent* thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);
    bool collisionCheckBS_OBB(CollisionComponent* thisCol, CollisionComponent *otherCol, TransformComponent *otherTrans, TransformComponent *thisTrans);

    ComponentSystem *compSys{nullptr};
};

#endif // COLLISIONSYSTEM_H
