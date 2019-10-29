#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H
#include "components.h"
#include "innpch.h"

class TransformComponent;
enum CollisionShape //trenger jeg de?
{
    //Konvekse
    AABB,
    OBB,
    BS
};
class CollisionComponent : public Components
{
public:
    CollisionComponent();


    //AABB
    //OBB - Object bounding box
    //BS Bounding Sphere
    //Trenger kun BS og OBB //lage BS og OBB i resourcefactory
    //Trenger størrelsen på objektet


    bool collisionCheckAABB(CollisionComponent* other, TransformComponent *otherTrans, TransformComponent *thisTrans);
    gsl::Vector3D collisionPosition;
    std::array <float,6> smallestBiggestXYZ;
    bool isRenderingBox = false;
    void setVertices(std::vector<Vertex> vertices){mVertices = vertices; verticesSize = mVertices.size();}
    void setVAO(GLuint VAO){mVAO = VAO;}
    void setVBO(GLuint VBO){mVBO = VBO;}
    GLuint &vAO(){return mVAO;}
    GLuint &vBO(){return mVBO;}
    std::vector<Vertex> vertices() const;
    CollisionShape shape;
     unsigned long long verticesSize;

     float getLengthX();
     float getLengthY();
     float getLengthZ();
     float radius();
     std::vector<int> indices() const;
     void setIndices(const std::vector<int> &indices);


private:
     float lengthX{0};
     float lengthY {0};
     float lengthZ{0};
     std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<int> mIndices;



    GLuint mVAO{0};
    GLuint mVBO{0};


    //Hvilken fasong


};

#endif // COLLISIONCOMPONENT_H
