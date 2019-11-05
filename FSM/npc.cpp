#include "npc.h"
#include "FSM/bsplinecurve.h"

NPC::NPC()
{
    mBSplineCurve = new BSplineCurve();
}

NPC::NPC(std::array<gsl::Vector3D, 2> EndP, std::vector<gsl::Vector3D> itemP)
{
    mBSplineCurve = new BSplineCurve();

    endPoints = EndP;

    items = itemP;
}

NPC::~NPC()
{
    delete  mBSplineCurve;
    mBSplineCurve = nullptr;
}

//----------------------CONTROLLPOINT-LOGIC___________________________________

//Sett kontrollpunkter med items og endepunkter 1.time
//Fylle inn kontrollpunker "controllPoints"
//for(int i = 0; i< itemSize(); i++)
//itemIndex for item {0,1,2,3...}
//når switch skjer, vet rekkefølgen, endre rekkefølge

//When a spline curve has n control points we need n+d +1 knots
//since the curve is constructed by blending control points with B-spline basis functions:

//n control points c0, c1,... cn−1

//Sette kontrollpunktene som skal sendes til b-Spline

//controllPoints.push_back(endPoints[0]);
//for(int i = 0; i <itemsIndex.size(); i++)
//{
//    controllPoints.push_back(items.at(itemsIndex.at(i)));
//}
//controllPoints.push_back(endPoints[1]);

//------------------IGNORE---------------------------------------------------------------

void NPC::setControlPoint()
{
   //clean up
    controllPoints.clear();
    itemsIndex.clear();

    controllPoints.push_back(endPoints[0]);
    for(int i = 0; i < items.size(); i++)
    {
        controllPoints.push_back(items.at(i));
    }

    controllPoints.push_back(endPoints[1]);
    for(int i= 0; i < items.size(); i++)
    {
        itemsIndex.push_back(i);
    }
}

void NPC::makeKnots()
{
    //Lag skjøter basert på antall controllpunker(endepunkter)
}

void NPC::patrol()
{
    //Vite om Kontrollpunkt, Endepunkt
    //if nådd endepunkt
    // notify(0)
    //StateChange
    //State: Patrol - Learn
    //If Item tatt
    //notify(1);
    //If player detected
    //notify(2);
    //If hinder i path
    //notify(3)

}

void NPC::learn()
{
    //

}

void NPC::updatePoints(std::array<gsl::Vector3D, 2> EndP, std::vector<gsl::Vector3D> itemP)
{
    endPoints = EndP;
    items = itemP;
    setControlPoint();
}

void NPC::notify(int notification)
{
    notification_queue.push(notification);

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

