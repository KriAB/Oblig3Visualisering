#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
class ComponentSystem;


class RenderSystem
{
public:
    RenderSystem(ComponentSystem *compSyst);
    void update();


private:
    ComponentSystem *mComponentSystem{nullptr};
};

#endif // RENDERSYSTEM_H
