#include "Entity.h"
#include "colorshader.h"
#include "textureshader.h"
#include "phongshader.h"

Entity::Entity(unsigned int mEId)
{
    eID = mEId;
}

Entity::Entity()
{

}
Entity::~Entity()
{
    delete child;
    child = nullptr;
    delete sibling;
    sibling = nullptr;

}
std::vector<Entity *> Entity::getChildren()
{
    std::vector<Entity*> children;

    if(child != nullptr)
    {
      children.push_back(child);

    //to get all the children, check for siblings
    while(child->sibling != nullptr)
    {
        children.push_back(sibling);
    }
    //Burde få tak i barna til child også osv.
    }

    return children;
}







