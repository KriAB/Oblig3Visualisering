#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"
#include "vector3d.h"
#include "gltypes.h"

class Vertex;

class OctahedronBall : public VisualObject {
public:
   OctahedronBall(int n=0);
   ~OctahedronBall() override;

   virtual void init() override;
   virtual void draw() override;

   GLint getRecursions() const {return mRecursions;}
   std::vector<Vertex> returnVertices(){return mVertices;}
   std::vector<GLuint> returnIndices(){return mIndices;}

private:
   GLint mRecursions;
   GLint mIndex;               //Used in recursion to build vertices

   GLuint mNumberOfVertices{0};

   void makeTriangle(const gsl::Vector3D &v1, const gsl::Vector3D &v2, const gsl::Vector3D &v3);
   void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, GLint n);
   void makeUnitOctahedron();
};

#endif // OCTAHEDRONBALL_H
