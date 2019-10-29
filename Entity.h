#ifndef ENTITY_H
#define ENTITY_H
#include <QOpenGLFunctions_4_1_Core>
#include "Components/components.h"
#include "Components/materialcomponent.h"
#include "Components/meshcomponent.h"
#include "texture.h"

class Entity : public QOpenGLFunctions_4_1_Core
{
public:

    //TODO: lage factory som gjør at objektene kun hentes en gang fra fil osv.
    Entity(unsigned int mEId);
    Entity();
    ~Entity();

    void setChild(Entity *mChild){child = mChild;}
    void setSibling(Entity *mSibling){sibling = mSibling;}

   // std::vector<Components*> getComponents(){return mComponents;}

    Entity* getChild(){return child;}
    Entity* getSibling(){return sibling;}
    std::vector<Entity*> getChildren();

    std::string mName;
    unsigned int eID; //Entity ID

private:

   // std::vector<Components*> mComponents; // sammle alle components som brukes i objectet.

    Entity *child = nullptr; //Peker til 1.child
    Entity *sibling = nullptr;// Peker til sibling




};

#endif // GAMEOBJECT_H
