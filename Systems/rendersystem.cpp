#include "rendersystem.h"
#include "componentsystem.h"
#include "rendercomponent.h"
RenderSystem::RenderSystem(ComponentSystem *compSyst): mComponentSystem(compSyst)
{

}

void RenderSystem::update()
{
    if(mComponentSystem->getRenderComponents().size() !=0)
    {
        for(auto renderComp : mComponentSystem->getRenderComponents())
        {
            if(renderComp->isRendering)
            {
                   renderComp->draw();
            }

        }
    }
}
