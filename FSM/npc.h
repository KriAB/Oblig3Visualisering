#ifndef NPC_H
#define NPC_H
#include "innpch.h"
#include <queue>

class BSplineCurve;

enum NPCStates
{
    SLEEP,PATROL, LEARN, CHASE
};
enum NPSEvents
{
    ENDPOINT_ARRIVED, ITEM_TAKEN, PLAYER_DETECTED, OBSTACLE_DETECTED
};
//Det er en FSM så kan kun gjøre en ting om gangen.
class NPC
{
public:
    NPC();
    ~NPC();
    //FSM part (Final State Machine)
    //
    void patrol(); //når den har gått en hel runde, kan den lære at den skal gå en annen rute neste gang.
    void learn();
    void chase();

    // Drawing part, jeg trenger ikke dette...
    void init();
    void draw();


    void sortIndex();
private:
    BSplineCurve * mBSplineCurve;
    gsl::Vector3D playerPosition;

    //FSM part
    int state{0};
    void notify(int notification); //Må gå til endepunkt før den endrer på kurven, slik som når den skal chase.
    std::queue<int> notification_queue;
    float elapsed_time;
    //B-spline kurven har en skøtvektor, f.eks. {0,0,0,1,2,3,3,3}
    //elapsed_time er en parameter:
    // 0 <= elapsed_time <3 i dette tilfellet

    void build_path();
    // Lage en (ny) bane for patruljering
    //gjøre en permutasjon av kontrollpunktene(kun items, ikke endepunkter) Dette vil si at man bytter om på rekkefølgen til kontrollpunktene
    //vikting å ikke endre endepunktene

    gsl::Vector3D endPoints[2];
    std::vector<gsl::Vector3D> items;
    std::vector<int> itemsIndex;
    std::vector<gsl::Vector3D> controllPoints;
    //for build_path();
    int iterator{0};

};

#endif // NPC_H
