#include "npc.h"
#include "FSM/bsplinecurve.h"

NPC::NPC()
{
    mBSplineCurve = new BSplineCurve();
}

NPC::~NPC()
{
    delete  mBSplineCurve;
    mBSplineCurve = nullptr;
}

void NPC::build_path()
{

    // Bytte rekkefølge på ItemsIndex.
    if(iterator < itemsIndex.size()-1)
  {
        std::iter_swap(itemsIndex.begin()+iterator, itemsIndex.begin()+iterator+1);
        iterator++;
  }
    //Når man har gått igjennom alle indeksene så resette den.
    else
    {
        iterator = 0;
        std::sort(itemsIndex.begin(), itemsIndex.end());

    }
    controllPoints.clear();
    controllPoints.resize(0);

    //Sette kontrollpunktene som skal sendes til b-Spline
    controllPoints.push_back(endPoints[0]);
    for(int i = 0; i <itemsIndex.size(); i++)
    {
        controllPoints.push_back(items.at(itemsIndex.at(i)));
    }
    controllPoints.push_back(endPoints[1]);



}

