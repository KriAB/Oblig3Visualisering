#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include "Components/components.h"
#include "innpch.h"

//Trenger egentlig ikke denne. dette kan inn i renderComponent
class MeshComponent : public Components
{
public:
    MeshComponent();
    //void receive(int message);

    void setVAO(GLuint VAO) {mVAO = VAO;}
    void setVBO(GLuint VBO){mVBO = VBO;}
    void setEAB(GLuint EAB){mEAB = EAB;}



    GLuint &vAO();

    GLuint &vBO();

    GLuint &eAB();


    bool drawTriangles = true;
//    GLuint VAO() const{return mVAO;}
//    GLuint VBO() const{return mVBO;}
//    GLuint EAB() const{return mEAB;}

    std::string name;

    std::vector<Vertex> vertices() const;
    void setVertices(const std::vector<Vertex> &vertices);

    std::vector<GLuint> indices() const;
    void setIndices(const std::vector<GLuint> &indices);
      void setSmalestBiggestXYZ(std::array<float,6> mSmallestBiggestXYZ){smallestBiggestXYZ = mSmallestBiggestXYZ;}
    unsigned long long verticesSize;
    unsigned long long indicesSize;
    std::array <float,6> getSmallestBiggestXYZ(){return smallestBiggestXYZ;}
    float getLengthX();
    float getLengthY();
    float getLengthZ();
protected:

    std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mIndices;    //This is usually not needed after object is made

    std::array <float,6> smallestBiggestXYZ;



    //den trenger kanskje en shape?
    //Eller sende inn hvilke objekt
    //Trenger antall MVertices og antall Indices.

    GLuint mVAO{0}; //i MeshComponent
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)


};


#endif // MESHCOMPONENT_H
