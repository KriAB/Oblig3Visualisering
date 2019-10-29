#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
class ComponentSystem;

class TransformSystem
{
public:
    TransformSystem(ComponentSystem *compSyst);
    void update();

private:

    ComponentSystem *mComponentSystem;
};

#endif // TRANSFORMSYSTEM_H
