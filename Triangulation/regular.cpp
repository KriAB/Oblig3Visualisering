#include "regular.h"
#include <algorithm>
#include <istream>
#include <fstream>
Regular::Regular()
{

}

Regular::Regular(std::vector<gsl::Vector3D> LAS)
{
    LASCoords = LAS;
}
//En treig sorterings algoritme, må finne en bedre
void Regular::sort()
{
    minX = LASCoords.at(0).x;
    maxX = LASCoords.at(0).x;
    minY = LASCoords.at(0).y;
    maxY = LASCoords.at(0).y;
    minZ = LASCoords.at(0).z;
    maxZ = LASCoords.at(0).z;

    for( int startIndex = 0; startIndex < LASCoords.size(); ++ startIndex)
    {
        int smallestIndex = startIndex;
        for(int currIndex = startIndex + 1; currIndex < LASCoords.size(); ++currIndex)
        {
            if(compare(LASCoords[currIndex], LASCoords[smallestIndex]) == true)
            {
                smallestIndex = currIndex;
            }
        }
        std::swap(LASCoords[startIndex], LASCoords[smallestIndex]);

        //sette min og max verdier
        if(LASCoords.at(startIndex).x < minX)
            minX = LASCoords.at(startIndex).x;
        if(LASCoords.at(startIndex).x > maxX)
            maxX = LASCoords.at(startIndex).x;

        if(LASCoords.at(startIndex).y < minY)
            minY = LASCoords.at(startIndex).y;
        if(LASCoords.at(startIndex).y > maxY)
            maxY = LASCoords.at(startIndex).y;

        if(LASCoords.at(startIndex).z < minZ)
            minZ = LASCoords.at(startIndex).z;
        if(LASCoords.at(startIndex).z > maxZ)
            maxZ = LASCoords.at(startIndex).z;

    }

    writeLASToTxtFileVector3("sortedNormMinMax20.txt");
}
void Regular::writeLASToTxtFileVector3(std::string filename)
{
    // filename = "32LasFileNormFlippedSortedMinMax.txt";
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {
        auto n =LASCoords.size();
        gsl::Vector3D vec3Min{minX,minY,minZ};
        gsl::Vector3D vec3Max{maxX,maxY,maxZ};
        ut << n << std::endl;
        ut << vec3Min<< std::endl;
        ut << vec3Max<< std::endl;
        for (auto it=LASCoords.begin(); it != LASCoords.end(); ++it)
        {
            gsl::Vector3D temp{it->x,it->y,it->z};
            ut << temp <<std::endl;
        }

        ut.close();
    }
}
void Regular::readTxtFileWithNeighbours(std::string filename)
{
    std::ifstream inn;
    filename = "sortedNormMinMax.txt";
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    inn.open(fileWithPath);
    std::vector<gsl::Vector3D> minMax;
    if (inn.is_open()) {
        int numberOfVertices;

        //  int numberOfNeighbours;
        gsl::Vector3D vertex;
        gsl::Vector3D min;
        gsl::Vector3D max;


        minMax.reserve(2);
        inn >> numberOfVertices;
        inn >> min;
        minMax.push_back(min);
        inn >> max;
        minMax.push_back(max);


        LASCoords.reserve(numberOfVertices);
        //neighbours.reserve(numberOfNeighbours);
        for (int i=0; i<numberOfVertices; i++) {
            inn >> vertex;
            LASCoords.push_back(vertex);
        }

        //        for (int i=0; i<numberOfNeighbours; i++) {
        //            inn >> neigh;
        //            neighbours.push_back(neigh);
        //        }
        minX = minMax.at(0).x;
        minY = minMax.at(0).y;
        minZ = minMax.at(0).z;
        maxX = minMax.at(1).x;
        maxY = minMax.at(1).y;
        maxZ = minMax.at(1).z;
        inn.close();
        qDebug() << "Factory file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }

}

std::vector<gsl::Vector3D> Regular::getNeighbours() const
{
    return neighbours;
}
bool Regular::compare(gsl::Vector3D a, gsl::Vector3D b)
{
    double epsilon = 0.00001;
    //sjekke om ax er mindre en bx                                              //Prosjektet har y opp, så bruker z her
    if(a.x < b.x ||(fabs (static_cast<double>(a.x-b.x)) < static_cast<double>(a.x)* epsilon && a.z <b.z))
        return true;

    return false;
}

void Regular::makeTriangles(unsigned int numInterval, unsigned int numPoints)
{
    gridCoords.reserve(numInterval*numInterval);
    // LASCoords.reserve(numPoints);
    // readTxtFileWithNeighbours("sortedNormMinMax.txt");

    findMinMax();

    if(numInterval < 2)
    {
        std::cout << "Number of Interval is to small for the triangulation!"<< std::endl;
    }
    if(LASCoords.size() == 0)
    {
        std::cout << "No LAS coordinates!" << std::endl;
    }
    else
    {
        //- Skal lage et rutenett basert på min og max verdier og delt opp i x interval.
        //- Først sortere punktene.
        // sort();

        //- Dele opp i x antall deler (interval) mellom hvert punkt
        calcLength();
        intervalX = lengthX/numInterval;
        intervalZ = lengthZ/numInterval;
        // - lage standard punkter, som ikke er i noen triangler enda.
        makeGridPoints(numInterval);
        // - finne nærmeste punkt fra LAS til hvert egensatte punkt og bruke høyden til LAS punktet
        //Jo større interval, jo likere LAS planet. Men det vil ta veldig lang tid.
        setHeight();
        // - lage  triangler, kunne brukt indices?

        //man vet antal punkt på hver linje, for det er numInterval
        //så finne en sammenheng
        std::vector <gsl::Vector3D> list1;
        std::vector <gsl::Vector3D> list2;
        unsigned int numTemp =0;
        //Sette liste 1 og 2 først
        //liste 1 putter du første linje med punktene
        //liste 2 putter du andre linje med punktene
        for(unsigned int j = numTemp; j < numInterval; j++)
        {
            list1.push_back(gridCoords.at(j));
            list2.push_back(gridCoords.at(j+numInterval));
            numTemp++;
        }
        int n = 0;
        numTemp = (numTemp*2);

        //    std::cout << "Length x: " << lengthX << " length y: " << lengthY <<  " Length z: " << lengthZ << std::endl;
        //    std::cout <<  " Min x " << minX << " Max x " << maxX << " Min y " << minY << " Max y "<< maxY <<" Min z " << minZ << " Max z " << maxZ << " Interval x " << intervalX<< " Interval z " << intervalZ << std::endl;
        //    std::cout << "Interval x: " << intervalX << " Interval z: " << intervalZ << std::endl;

        int numForNb = 0;
        for(unsigned int i = 0; i < numInterval-1; i++)
        {
            //Noe galt med List laginga?
            //Triangulerer så de to listene med hverandre.
            //T0: L10 L20 L21 T1: L10 L21 L11,
            for(unsigned int j = 0; j <numInterval-1; j++)
            {/*

                //  std::cout << "T" << n << " | " << list1.at(j).x-minX <<" " << list1.at(j).y - minY<< " " << list1.at(j).z-minZ <<" | " << list2.at(j).x -minX<< " " << list2.at(j).y -minY<< "," << list2.at(j).z -minZ<< " | " << list2.at(j+1).x -minX << " " << list2.at(j+1).y -minY<< "," << list2.at(j+1).z - minZ<< std::endl;
                //må ha noe if greier

                //for første linje
                if(i == 0)
                {
                    if(j == 0)
                    {
                        //T0
                        neighbours.push_back(gsl::Vector3D(-1,numForNb+1,numForNb+3));
                        //T1
                        neighbours.push_back(gsl::Vector3D(numForNb,-1,numInterval-1));
                    }
                    else if (j == numInterval)
                    {
                        //TNumInterval0
                        neighbours.push_back(gsl::Vector3D(-1,numForNb+1,-1));
                        //TNumInterval1
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                    }
                    else
                    {
                        //T2...
                        neighbours.push_back(gsl::Vector3D(numForNb+3,numForNb+1,-1));
                        //T3...
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                    }
                }
                //for siste linje
                else if(i == numInterval)
                {
                    if(j == 0)
                    {
                        //T0
                        neighbours.push_back(gsl::Vector3D(numForNb +1 -numInterval ,numForNb+1,numForNb+3));
                        //T1
                        neighbours.push_back(gsl::Vector3D(-1,-1,numForNb));
                    }
                    else if (j == numInterval)
                    {
                        //TNumInterval0
                        neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,-1));
                        //TNumInterval1
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,-1));
                    }
                    else
                    {
                        //T2...
                        neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                        //T3...
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,-1));
                    }
                }
                else
                {
                    if(j == 0)
                    {
                        //T0
                        neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                        //T1
                        neighbours.push_back(gsl::Vector3D(numForNb,-1,numForNb+numInterval));

                    }
                    if(j == numInterval)
                    {
                        //TNumInterval0
                        neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,-1));
                        //TNumInterval1
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));

                    }
                    else
                    {
                        //T2...
                        neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                        //T3...
                        neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                    }

                }
                numForNb++;*/

                finalTriangles.push_back(gsl::Vector3D(list2.at(j+1).x, list2.at(j+1).y, list2.at(j+1).z));
                finalTriangles.push_back(gsl::Vector3D(list2.at(j).x, list2.at(j).y, list2.at(j).z));
                finalTriangles.push_back(gsl::Vector3D(list1.at(j).x,list1.at(j).y, list1.at(j).z));
                n++;
                //            std::cout << "T" << n << " | "<< list1.at(j).x -minX<<" "<< list1.at(j).y-minY << " " << list1.at(j).z -minZ<<" | " << list2.at(j+1).x -minX<<  " " <<list2.at(i+1).y-minY <<  "," << list2.at(j+1).z-minZ << " | " << list1.at(j+1).x -minX<< " " << list1.at(j+1).y -minY<< "," << list1.at(j+1).z -minZ<< std::endl;

                finalTriangles.push_back(gsl::Vector3D(list1.at(j+1).x, list1.at(j+1).y, list1.at(j+1).z));
                finalTriangles.push_back(gsl::Vector3D(list2.at(j+1).x, list2.at(j+1).y, list2.at(j+1).z));
                finalTriangles.push_back(gsl::Vector3D(list1.at(j).x,list1.at(j).y, list1.at(j).z));
                n++;
            }
            //  std::cout << "Num interval" << numInterval << " Size: " << tempCoords.size() << " list1 size: " << list1.size() << "list2 size: " <<list2.size() <<std::endl;
            //så setter man liste 1 = liste 2 og tømmer liste 2.
            list1.empty();
            list1.resize(0);
            list1 = list2;
            list2.empty();
            list2.resize(0);
            //så setter man liste 2 til linje 3 osv.

            for(unsigned int j = 0; j < numInterval; j++)
            {
                if(numTemp+j < gridCoords.size())
                {
                    list2.push_back(gridCoords.at(numTemp+j));
                }

            }
            //   std::cout << "new List2 size:" <<list2.size() << std::endl;
            if(list2.size() != numInterval)
            {
                std::cout << "Something wrong with Triangulation" << std::endl;
                break;
            }
            numTemp = numTemp+numInterval;


        }
    }

    makeNeighbours(numInterval);
}
void Regular::makeGridPoints(unsigned int numInterval)
{
    //lage punktene bortover z aksen, så øke x, så lage de neste punktene
    float tempIntX = minX; //  + intervalX
    float tempIntZ = minZ; // + intervalZ;
    for(unsigned int i = 0; i < numInterval; i++)
    {
        for(unsigned int j = 0; j < numInterval; j++)
        {
            //  std::cout << gsl::Vector3D(tempIntX,0,tempIntZ) <<std::endl;
            gridCoords.push_back(gsl::Vector3D(tempIntX, 0, tempIntZ));
            tempIntZ = tempIntZ+intervalZ;
        }
        tempIntZ = minZ;
        tempIntX = tempIntX+intervalX;
    }

}
//fungerer ikke helt
void Regular::setHeight()
{
    gsl::Vector3D newCoord = LASCoords.at(0);
    float lengthBetABx = fabs(gridCoords.at(0).x - newCoord.x);
    float lengthBetABz = fabs(gridCoords.at(0).z - newCoord.z);
    float lengthBetABTempX;
    float lengthBetABTempZ;
    //Finne punktene som er nærmest de faste punktene
    int atNum = 1;
    int tempNum = 0;

    //Må finne den korteste lengden i hele lista.

    for(int i = 0; i< gridCoords.size(); i++)
    {
        lengthBetABx = fabs(gridCoords.at(i).x - newCoord.x);
        lengthBetABz = fabs(gridCoords.at(i).z - newCoord.z);

        for(int j = atNum; j < LASCoords.size()-1; j++)
        {
            lengthBetABTempX = fabs(gridCoords.at(i).x - LASCoords.at(j).x);
            lengthBetABTempZ = fabs(gridCoords.at(i).z - LASCoords.at(j).z);
            if(lengthBetABTempX <= lengthBetABx && lengthBetABTempZ <= lengthBetABz)
            {
                newCoord = LASCoords.at(j);
                lengthBetABx = lengthBetABTempX;
                lengthBetABz = lengthBetABTempZ;
            }
            // atNum++;

        }
        gridCoords.at(i).y = newCoord.y;
        newCoord = LASCoords.at(0);
        lengthBetABx = fabs(gridCoords.at(0).x - newCoord.x);
        lengthBetABz = fabs(gridCoords.at(0).z - newCoord.z);

        std::cout << "Height for point: " << i << std::endl;
    }
    minY = gridCoords.at(0).y;
    maxY = gridCoords.at(0).y;
    for(int i = 0; i<gridCoords.size(); i++)
    {
        if(minY > gridCoords.at(i).y)
        {
            minY = gridCoords.at(i).y;
        }
        if(maxY < gridCoords.at(i).y)
        {
            maxY = gridCoords.at(i).y;
        }
    }

}

void Regular::findMinMax()
{

    for(int i=0; i< LASCoords.size(); i++)
    {
        //sette min og max verdier
        if(LASCoords.at(i).x < minX)
            minX = LASCoords.at(i).x;
        if(LASCoords.at(i).x > maxX)
            maxX = LASCoords.at(i).x;

        if(LASCoords.at(i).y < minY)
            minY = LASCoords.at(i).y;
        if(LASCoords.at(i).y > maxY)
            maxY = LASCoords.at(i).y;

        if(LASCoords.at(i).z < minZ)
            minZ = LASCoords.at(i).z;
        if(LASCoords.at(i).z > maxZ)
            maxZ = LASCoords.at(i).z;
    }
}

void Regular::makeNeighbours(int numInterval)
{

    int numForNb = 0;
    int numOfTriangles = finalTriangles.size()/3;

    for(unsigned int i = 0; i < numInterval-1; i++)
    {
        for(unsigned int j = 0; j <numInterval-1; j++)
        {

            //  std::cout << "T" << n << " | " << list1.at(j).x-minX <<" " << list1.at(j).y - minY<< " " << list1.at(j).z-minZ <<" | " << list2.at(j).x -minX<< " " << list2.at(j).y -minY<< "," << list2.at(j).z -minZ<< " | " << list2.at(j+1).x -minX << " " << list2.at(j+1).y -minY<< "," << list2.at(j+1).z - minZ<< std::endl;
            //må ha noe if greier

            //for første linje
            if(i == 0)
            {
                if(j == 0)
                {
                    //T0
                    neighbours.push_back(gsl::Vector3D(-1,numForNb+1,numForNb+3));
                    //T1
                    neighbours.push_back(gsl::Vector3D(numForNb,-1,numInterval-1));
                }
                else if (j == numInterval)
                {
                    //TNumInterval0
                    neighbours.push_back(gsl::Vector3D(-1,numForNb+1,-1));
                    //TNumInterval1
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                }
                else
                {
                    //T2...
                    neighbours.push_back(gsl::Vector3D(numForNb+3,numForNb+1,-1));
                    //T3...
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                }
            }
            //for siste linje
            else if(i == numInterval)
            {
                if(j == 0)
                {
                    //T0
                    neighbours.push_back(gsl::Vector3D(numForNb +1 -numInterval ,numForNb+1,numForNb+3));
                    //T1
                    neighbours.push_back(gsl::Vector3D(-1,-1,numForNb));
                }
                else if (j == numInterval)
                {
                    //TNumInterval0
                    neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,-1));
                    //TNumInterval1
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,-1));
                }
                else
                {
                    //T2...
                    neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                    //T3...
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,-1));
                }
            }
            //Det som er i midten
            else
            {
                if(j == 0)
                {
                    //T0
                    neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                    //T1
                    neighbours.push_back(gsl::Vector3D(numForNb,-1,numForNb+numInterval));

                }
                if(j == numInterval)
                {
                    //TNumInterval0
                    neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,-1));
                    //TNumInterval1
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));

                }
                else
                {
                    //T2...
                    neighbours.push_back(gsl::Vector3D(-numInterval+1+numForNb,numForNb+1,numForNb+3));
                    //T3...
                    neighbours.push_back(gsl::Vector3D(numForNb,numForNb-2,numForNb+numInterval));
                }

            }
            numForNb++;
        }
    }
    if(neighbourSize() != numOfTriangles)
    {
        std::cout << "Something wrong with neighbours, not same size as number of Triangles" << std::endl;
    }

}

void Regular::calcLength()
{
    lengthX = maxX -minX;
    lengthY = maxY -minY; //trenger vel egentlig ikke denne?
    lengthZ = maxZ - minZ;
}

std::vector<gsl::Vector3D> Regular::getFinalTriangles() const
{
    return finalTriangles;
}

void Regular::setLASCoords(std::vector<gsl::Vector3D> value)
{
    LASCoords = value;
    writeLASToTxtFileVector3("temptemp10000.txt");
}
