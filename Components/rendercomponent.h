#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Components/components.h"
#include <QOpenGLFunctions_4_1_Core>
class MaterialComponent;
class MeshComponent;
class TransformComponent;

class RenderComponent : public Components, public QOpenGLFunctions_4_1_Core
{
public:
    RenderComponent(int EID, MeshComponent* mesh, MaterialComponent* material, TransformComponent *transform);

    void draw();

    void drawArrayTriangles();
    void drawIndicesTriangle();
    void drawArrayLines();
    void drawIndicesLines();
    void drawArrayLineLoop();
    void drawIndicesLineLoop();
private:
    MaterialComponent *matComp{nullptr};
    MeshComponent *meshComp{nullptr};
    TransformComponent *transComp{nullptr};
};

#endif // RENDERCOMPONENT_H
