#include "npc.h"
#include "FSM/bsplinecurve.h"
#include "transformcomponent.h"
#include "componentsystem.h"
#include "systemmanager.h"

NPC::NPC(std::array<gsl::Vector3D, 2> EndP, std::vector<gsl::Vector3D> itemP, SystemManager * systemManager)
{
    mBSplineCurve = new BSplineCurve();

    mSystemManager = systemManager;
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

void NPC::gameRunner(int numberOfPoints)
{
    // mNumberOfPoints = numberOfPoints;

    if(state == 0)
    {
        //sleep
    }
    if(state == 1)
    {
        patrol();
    }
    if(state == 2)
    {
        learn();
        indexInPath = 0;
    }

}


//Lag skjøter basert på antall controllpunker(endepunkter)
//Antall knots = n + d +1 når t={t0}^n+d
//Vector knot
//n = antall kontrollpunkter
//d = degree

void NPC::patrol()
{
    //Vite om Kontrollpunkt, Endepunkt
    //if nådd endepunkt
    // notify(0)
    //StateChange
    //State: Patrol - Learn

    //Hvis Item tatt
    if(isItemTaken== true)
    {
        notify(1);
        isItemTaken = false;
    }

    //If player detected
    //notify(2);
    //If hinder i path
    //notify(3)

    if(indexInPath == path.size()-1)
    {
        //Reached end point
        notify(0);
        state = 2;
    }
    //Move
    mSystemManager->componentSystem()->getTransCompWithEId(mSystemManager->getNumNPC())->setPosition(path.at(indexInPath));
    indexInPath++;
}

void NPC::learn()
{
    while(!notification_queue.empty())
    { int temp = notification_queue.front();
        notification_queue.pop();

        if(temp == 0)
        {
            //Nådd endepunkt
            build_path_Item_not_taken();
        }
        if(temp == 1)
        {
            //item taken
            build_path_Item_taken();
        }
        //No more items
        if(items.size() == 0)
        {
            state = 0;
            break;
        }
    }
    state = 1;

}

void NPC::updatePoints(std::vector<gsl::Vector3D> itemP)
{
    items = itemP;
    setControlPoint();
}

void NPC::notify(int notification)
{
    notification_queue.push(notification);

}


void NPC::build_path_Item_not_taken() //If item not taken
{
   // items = mSystemManager->getItemsPosition();
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

    //Bytte rekkefølge på endPts

    std::iter_swap(endPoints.begin(),endPoints.end()-1);

    //Sette kontrollpunktene som skal sendes til b-Spline
    controllPoints.push_back(endPoints[0]);
    if(!items.empty())
        std::cout << "Items empty";

    for(int i = 0; i <itemsIndex.size(); i++)
    {
        controllPoints.push_back(items.at(itemsIndex.at(i)));
    }
    controllPoints.push_back(endPoints[1]);

    make_path(mNumberOfPoints);
}

void NPC::build_path_Item_taken()
{
    setControlPoint();
    controllPoints.clear();
    controllPoints.resize(0);

    //Bytte rekkefølge på endPts
    std::iter_swap(endPoints.begin(),endPoints.end()-1);

    //Sette kontrollpunktene som skal sendes til b-Spline
    controllPoints.push_back(endPoints[0]);
    for(int i = 0; i <itemsIndex.size(); i++)
    {
        controllPoints.push_back(items.at(itemsIndex.at(i)));
    }
    controllPoints.push_back(endPoints[1]);

    make_path(mNumberOfPoints);
}

std::vector<gsl::Vector3D> NPC::make_path(int numberOfPoints)
{
    mNumberOfPoints = numberOfPoints;
    float x = 1.0f/numberOfPoints;
    path.clear();
    for(int i = 0; i < numberOfPoints; i++)
    {
        path.push_back(mBSplineCurve->makePatrolPoint(x*i, controllPoints));
    }
    return path;
}

