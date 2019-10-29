#ifndef REGULAR_H
#define REGULAR_H
#include <vector>
#include "innpch.h"

//Sparer på det punktet som er nærmest det faste satte punktet.
class Regular
{
public:
    Regular();
    Regular(std::vector<gsl::Vector3D> LAS);
    void sort();
    //sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også
    bool compare(gsl::Vector3D a,gsl::Vector3D b);
    void makeTriangles(unsigned int numInterval, unsigned int numPoints);
    void makeGridPoints(unsigned int numInterval);
    void setHeight();
    void findMinMax();
    void makeNeighbours(int numInterval);

    void writeLASToTxtFileVector3(std::string filename);

    int triangleSize() {return finalTriangles.size();}
    int neighbourSize(){return neighbours.size();}
void calcLength();

std::vector<gsl::Vector3D> getFinalTriangles() const;

void setLASCoords(std::vector<gsl::Vector3D> value);

void readTxtFileWithNeighbours(std::string filename);
std::vector<gsl::Vector3D> getNeighbours() const;

private:
std::vector<gsl::Vector3D> LASCoords;
std::vector<gsl::Vector3D> finalTriangles;
    std::vector<gsl::Vector3D> gridCoords;
    std::vector<gsl::Vector3D>  neighbours;

    float minX{0};
    float maxX{0};
    float minY{0};
    float maxY{0};
    float minZ{0};
    float maxZ{0};

    float lengthX;
    float lengthY;
    float lengthZ;

    float intervalX;
    float intervalZ;









};

#endif // REGULAR_H
