#ifndef GRAVITY_H
#define GRAVITY_H
#include "innpch.h"


class Gravity
{
public:
    Gravity();

    bool update(gsl::Vector3D mTargetCoordinates);
    void newtonsSecondLawAcceleration(); //Ligning 7
    void setTarget(gsl::Vector3D mTargetCoordinates){targetCoordinates = mTargetCoordinates;}
    void checkBarycentricCoordinates(gsl::Vector3D mTargetCoordinates);
    void checkBarycentricCoordinatesNoNeighbours(gsl::Vector3D mTargetCoordinates);
    gsl::Vector3D calcNormalForce();
    gsl::Vector3D calcGravityForce();
    float calcHeight(gsl::Vector3D baryCoor, gsl::Vector3D p1, gsl::Vector3D p2, gsl::Vector3D p3);
    //Calc barysentric coordinates:

    gsl::Vector3D calcNormal(gsl::Vector3D baryCoor, gsl::Vector3D p1, gsl::Vector3D p2, gsl::Vector3D p3);
    void setTriangles(std::vector<Vertex> vertices,  std::vector<gsl::Vector3D> mNeighbours);
    void calcAlpha();
    float getHeight() const;

    void setRadius(float value);

    gsl::Vector3D getAcceleration() const;

    bool isOnTriangle = true;

    int numOfRounds = 0;
private:
    float height = 1;
    float radius = 1.2;
    gsl::Vector3D acceleration;
    gsl::Vector3D gravityForce; //force
    gsl::Vector3D targetCoordinates;
    float mass = 1;
    gsl::Vector3D normal; //normalen i kontaktpunktet med underlaget
    gsl::Vector3D normalForce;
    double alpha;

    gsl::Vector3D contactTriangle; //rekkefølge mot urviseren v0,v1,v2
    std::vector<gsl::Vector3D> trianglePoints;
     std::vector<gsl::Vector3D> neighbours;
     bool foundTriangle = false;
};

//TODOs
//1 Beregne normalvektoren i kontaktpunktet med underlaget
//2 Beregne Akselerasjonsvektoren til kula etter ligning 7
//3 Oppdater ballens hastighet
//4 (Beregn ballens rotasjonsvektor)
//5 (Beregn ballens rotasjon)
//6 Oppdater ballens posisjon

//HowTODOs:
//Identifiser hvilken trekant ballen er på (med barysentriske koordinater)
//Trenger hjørnene (verices) i rekkefølge mot urviseren v0, v1, v2
//Normalvektor N =v0v1=(v1-v0)x(v2-v0)
//normaliser
#endif // GRAVITY_H
