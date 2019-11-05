#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <vector>

class RenderSystem;
class ComponentSystem;
class InputSystem;
class CollisionSystem;
class TransformSystem;
class TransformComponent;

class Shader;
class MainWindow;
class RenderWindow;
class NPC;


class SystemManager
{
public:
    SystemManager(RenderWindow *renderwindow = nullptr, MainWindow *mainwindow = nullptr, Shader *shaderProgram = nullptr);
     ~ SystemManager();
    void update();

    ComponentSystem *componentSystem();

    RenderSystem *renderSystem() const;

    void setNPC(int value);

    void setPlayer(int value);

    void setItems(const std::vector<TransformComponent*> &value);

    void setEndPts(const std::array<gsl::Vector3D, 2> &value);

    void setItemsPosition();

    //Oblig 3
    void checkPlayerInput();
    void calcNPCBehavior();
    bool checkCollision(int EID, int otherEID);
    bool collisionHandling(int otherEID, int index);
private:
    RenderSystem * mRenderSystem{nullptr};
    ComponentSystem * mComponentSystem{nullptr};
    InputSystem *mInputSystem{nullptr};
    CollisionSystem * mCollisionSystem{nullptr};
    TransformSystem * mTransformSystem{nullptr};

    MainWindow *mMainWindow{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    Shader *mShaderProgram[4]{nullptr};



    int player{-1};
    int numNPC{-1};

    std::vector<TransformComponent*> items;
    std::vector<gsl::Vector3D> itemsPosition;
    std::array<gsl::Vector3D,2> endPts{gsl::Vector3D{0,2,0}, gsl::Vector3D(20,2,20)};

    //To limit the calculation, so that it does not lag.
    int intervalCalculationHeight{0};
    NPC *mNPC;
};

#endif // SYSTEMMANAGER_H
