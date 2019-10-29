#include "meshcomponent.h"

MeshComponent::MeshComponent() : Components (compType::MESH)
{

}



GLuint &MeshComponent::vAO()
{
    return mVAO;
}

GLuint &MeshComponent::vBO()
{
    return mVBO;
}

GLuint &MeshComponent::eAB()
{
    return mEAB;
}

std::vector<Vertex> MeshComponent::vertices() const
{
    return mVertices;
}

void MeshComponent::setVertices(const std::vector<Vertex> &vertices)
{
    mVertices = vertices;
    verticesSize = mVertices.size();
}

std::vector<GLuint> MeshComponent::indices() const
{
    return mIndices;
}

void MeshComponent::setIndices(const std::vector<GLuint> &indices)
{
    mIndices = indices;
    indicesSize = mIndices.size();
}

float MeshComponent::getLengthX()
{
    float xMin = smallestBiggestXYZ.at(0);
    float xMax = smallestBiggestXYZ.at(1);
    return xMax - xMin;
}

float MeshComponent::getLengthY()
{
    float yMin =smallestBiggestXYZ.at(2);
    float yMax = smallestBiggestXYZ.at(3);
    return  yMax - yMin;
}
float MeshComponent::getLengthZ()
{
    float zMin = smallestBiggestXYZ.at(4);
    float zMax = smallestBiggestXYZ.at(5);
    return zMax - zMin;
}

std::vector<gsl::Vector3D> MeshComponent::neighbours() const
{
    return mNeighbours;
}

void MeshComponent::setNeighbours(std::vector<gsl::Vector3D> nb)
{
    mNeighbours = nb;
}

float MeshComponent::radiusY()
{
    return abs(getLengthY()/2);
}
