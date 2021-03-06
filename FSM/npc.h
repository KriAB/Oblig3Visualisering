#ifndef NPC_H
#define NPC_H
#include "innpch.h"
#include <queue>
class SystemManager;

class BSplineCurve;

enum NPCStates
{
    SLEEP,PATROL, LEARN, CHASE
};
enum NPSEvents

        //      0               1               2               3

{
    ENDPOINT_ARRIVED, ITEM_TAKEN, PLAYER_DETECTED, OBSTACLE_DETECTED
};
//Det er en FSM så kan kun gjøre en ting om gangen.
class NPC
{
public:

    NPC(std::array<gsl::Vector3D,2> EndP, std::vector<gsl::Vector3D> itemP, SystemManager *systemManager);
    ~NPC();

    //Sett kontrollpunkter med items og endepunkter 1.time
    //Fylle inn kontrollpunker "controllPoints"
    void setControlPoint();


    void gameRunner(int numberOfPoints);

    //FSM part (Final State Machine)
    //
    void patrol(); //når den har gått en hel runde, kan den lære at den skal gå en annen rute neste gang.
    void learn();
    void chase();

    // Drawing part, jeg trenger ikke dette...
    void init();
    void draw();

    //Updating the points, if ex some is taken by the player etc. Should only be executed if a point is gone. Need to do this in SystemManager.
    void updatePoints(std::vector<gsl::Vector3D> itemP);

    void sortIndex();

    void build_path_Item_not_taken();
    void build_path_Item_taken();
    std::vector<gsl::Vector3D> make_path(int numberOfPoints);
    bool isItemTaken{false};
private:
    BSplineCurve * mBSplineCurve;
    gsl::Vector3D playerPosition;

    //FSM part
    int state{1};
    //NPC får sender en notifikasjon til seg selv hvis et object er borte
    //NPC sender notifikasjon om spiller oppdaget.
    //NPC sender notifikasjon om endepunkt er nådd.
    void notify(int notification); //Må gå til endepunkt før den endrer på kurven, slik som når den skal chase.
    //Notifikasjonene blir lagt i en kø, når NPC'en ikke kan sende en notifikasjon øyeblikkelig.
    std::queue<int> notification_queue;
    float elapsed_time;
    //B-spline kurven har en skøtvektor, f.eks. {0,0,0,1,2,3,3,3}
    //elapsed_time er en parameter:
    // 0 <= elapsed_time <3 i dette tilfellet


    // Lage en (ny) bane for patruljering
    //gjøre en permutasjon av kontrollpunktene(kun items, ikke endepunkter) Dette vil si at man bytter om på rekkefølgen til kontrollpunktene
    //vikting å ikke endre endepunktene


    //Sender inn array med items sin posisjon, og en array med endepunktene
    //item sine posisjoner = kontrollpunkter(endepunkter)

    //Endepunkter
  std::array<gsl::Vector3D,2> endPoints;
    //Vector med items to collect
    std::vector<gsl::Vector3D> items;
    std::vector<int> itemsIndex;
    //Kontrollpunkter
    std::vector<gsl::Vector3D> controllPoints;
    //for build_path();
    int iterator{0};

    std::vector<gsl::Vector3D> path;

    SystemManager *mSystemManager{nullptr};

  //For path
    int mNumberOfPoints;

    int indexInPath{0};

};

//

#endif // NPC_H
