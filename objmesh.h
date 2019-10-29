#ifndef OBJMESH_H
#define OBJMESH_H
#include "visualobject.h"


class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ObjMesh(std::string filename);
    ~ObjMesh() override;

    virtual void draw() override;
    virtual void init() override;

    void readFile(std::string filename);
};

#endif // OBJMESH_H
