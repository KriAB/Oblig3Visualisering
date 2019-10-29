#ifndef SKYBOX_H
#define SKYBOX_H

#include "visualobject.h"


class SkyBox : public VisualObject
{
public:
    SkyBox();

    virtual void init() override;
    virtual void draw() override;
};

#endif // SKYBOX_H
