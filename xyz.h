#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class XYZ : public VisualObject
{
public:
   XYZ();
   ~XYZ() override;
   void init() override;
   void draw() override;

private:

};

#endif // XYZ_H
