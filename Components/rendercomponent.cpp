#include "rendercomponent.h"
#include "materialcomponent.h"
#include "meshcomponent.h"
#include "transformcomponent.h"
#include <iostream>
RenderComponent::RenderComponent(int EID, MeshComponent *mesh, MaterialComponent *material, TransformComponent *transform) : Components (compType::RENDER)
{
    eID = EID;
    meshComp = mesh;
    matComp = material;
    transComp = transform;
}

void RenderComponent::draw()
{
    initializeOpenGLFunctions();

    if(meshComp == nullptr || matComp == nullptr|| transComp == nullptr)
    {
        std::cout << "Component missing for renderComponent " << eID;
    }
    else
    {
        glUseProgram(matComp->returnMaterial().mShader->getProgram());
        glBindVertexArray( meshComp->vAO() );
        matComp->returnMaterial().mShader->transmitUniformData(&transComp->matrix(), &matComp->returnMaterial());

        if(meshComp->indicesSize != 0)
        {
          if(meshComp->drawTriangles == true)
              drawIndicesTriangle();
          else
              drawIndicesLines();
        }
        else
        {
            if(meshComp->drawTriangles == true)
                drawArrayTriangles();
            else
                drawArrayLines();
         }
    }
}

void RenderComponent::drawArrayTriangles()
{
      glDrawArrays(GL_TRIANGLES, 0, meshComp->verticesSize);
}

void RenderComponent::drawIndicesTriangle()
{
    glDrawElements(GL_TRIANGLES, meshComp->indicesSize, GL_UNSIGNED_INT, nullptr);
}

void RenderComponent::drawArrayLines()
{
    glDrawArrays(GL_LINES, 0, meshComp->verticesSize);
}

void RenderComponent::drawIndicesLines()
{
    glDrawElements(GL_LINES, meshComp->indicesSize, GL_UNSIGNED_INT, nullptr);
}

void RenderComponent::drawArrayLineLoop()
{
      glDrawArrays(GL_LINE_LOOP, 0, meshComp->verticesSize);
}

void RenderComponent::drawIndicesLineLoop()
{
      glDrawElements(GL_LINE_LOOP, meshComp->indicesSize, GL_UNSIGNED_INT, nullptr);
}



