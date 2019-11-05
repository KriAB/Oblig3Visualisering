#include "gravity.h"

Gravity::Gravity()
{

}

bool Gravity::update(gsl::Vector3D mTargetCoordinates)
{

    if(neighbours.size() != 0)
    checkBarycentricCoordinates(mTargetCoordinates);


    if(isOnTriangle == true)
    {
        newtonsSecondLawAcceleration();

//        std::cout << "Normal            x: "<< normal.x << " y: " << normal.y << " z: " << normal.z << std::endl;
//        std::cout << "Acceleration:     x: " << acceleration.x << " y: " << acceleration.y << " z: " << acceleration.z << std::endl;
//        std::cout << "Gravity Force:    x: " << gravityForce.x << " y: " << gravityForce.y << " z: " << gravityForce.z << std::endl;
//        std::cout << "Normal Force:     x: " << normalForce.x <<" y: " << normalForce.y << " z: " << normalForce.z << std::endl;
//        std::cout << "Alpha :              " << alpha << std::endl;
    }

    return isOnTriangle;
}

void Gravity::newtonsSecondLawAcceleration()
{
    // a = F+G*1/m
    acceleration = (calcNormalForce()+gravityForce)*(1/mass);
}

void Gravity::checkBarycentricCoordinates(gsl::Vector3D mTargetCoordinates)
{ //isOnTriangle = false;
  //  setTarget(mTargetCoordinates, );
    gsl::Vector3D barCenCoor;
    gsl::Vector3D p1;
    gsl::Vector3D p2;
    gsl::Vector3D p3;

    gsl::Vector2D target(mTargetCoordinates.x,mTargetCoordinates.z);
    unsigned int nStartPositionTriangle = 0;
    unsigned int nTriangle= 0;

    if(trianglePoints.size() != 0)
    {
        p1 = trianglePoints.at(nStartPositionTriangle);
        p2 = trianglePoints.at(nStartPositionTriangle+1);
        p3 = trianglePoints.at(nStartPositionTriangle+2);

        //returnerer u v w
        //u = p2p3
        //v = p1p2
        //w = p1p3


        barCenCoor = target.barycentricCoordinates(gsl::Vector2D(p1.x,p1.z),gsl::Vector2D(p2.x,p2.z),gsl::Vector2D(p3.x,p3.z));
        GLfloat u = barCenCoor.x;
        GLfloat v = barCenCoor.y;
        GLfloat w = barCenCoor.z;
        while(foundTriangle == false)
        {
            if(trianglePoints.size()>=3)
            {

                if(barCenCoor.x >0 && barCenCoor.y > 0  && barCenCoor.z > 0)
                {
                    foundTriangle = true;
                    break;

                }
                else
                {   //For at det ikke skal gå i en evig loop, når target er på utsiden.
                    if(numOfRounds < trianglePoints.size())
                    {
                        numOfRounds++;
                        gsl::Vector3D n0(neighbours.at(nTriangle));
                        //Hvis u/p2p3 er minst
                        if((u <= v) &&  (u <= w))
                        {
                           // std::cout << " inside u" << std::endl;
                            if(static_cast<int>(n0.x) >= 0)
                            {
                                nTriangle = n0.x;
                            }
                            else if(static_cast<int>(n0.y) >= 0)
                            {
                                nTriangle = n0.y;
                            }
                            else if(static_cast<int>(n0.z) >= 0)
                            {
                                nTriangle = n0.z;
                            }
                            else
                            {
                                std::cout << "Something wrong p2p3" << std::endl;
                                break;
                            }

                        }
                        //Hvis v/p1p3 er minst
                        else if((v <= u) &&  (v <= w))
                        {
                            //std::cout << " inside v" << std::endl;
                            if(n0.y >= 0)
                            {
                                nTriangle = n0.y;
                            }
                            else if(n0.z >= 0)
                            {
                                nTriangle = n0.z;
                            }
                            else if(n0.x >= 0)
                            {
                                nTriangle = n0.x;
                            }
                            else
                            {
                                std::cout << "Something wrong p1p3" << std::endl;
                                break;
                            }

                        }
                        //Hvis w/p1p2 er minst
                        else if((w <= u) && (w <= v))
                        {
                           // std::cout << " inside w" << std::endl;
                            if(n0.z >= 0)
                            {
                                nTriangle = n0.z;
                            }
                            else if(n0.y >= 0)
                            {
                                nTriangle = n0.y;
                            }
                            else if(n0.x >= 0)
                            {
                                nTriangle = n0.x;
                            }
                            else
                            {
                                std::cout << "Something wrong p1p2" << std::endl;
                                break;
                            }

                        }
                    }
                    else
                    {
                        //Ikke innenfor noen
                        //std::cout << "Not whitin!" << std::endl;
                        isOnTriangle = false;
                        numOfRounds = 0;
                        break;
                    }
                    nStartPositionTriangle = nTriangle*3;
                    p1 = trianglePoints.at(nStartPositionTriangle);
                    p2 = trianglePoints.at(nStartPositionTriangle+1);
                    p3 = trianglePoints.at(nStartPositionTriangle+2);

                    barCenCoor = target.barycentricCoordinates(gsl::Vector2D(p1.x,p1.z),gsl::Vector2D(p2.x,p2.z),gsl::Vector2D(p3.x,p3.z));
                    u = barCenCoor.x;
                    v = barCenCoor.y;
                    w = barCenCoor.z;


                }

//                std::cout << std::endl
//                          << "Triangle Nr: " << nTriangle
//                          << " Barycentric Coordinates: u: " << barCenCoor.x << " v: " << barCenCoor.y << " w: " <<barCenCoor.z << std::endl
//                          << " Neighbors at x: " << neighbours.at(nTriangle).x << " y: " << neighbours.at(nTriangle).y << " z: " << neighbours.at(nTriangle).z << std::endl
//                          << " v1 x: " <<trianglePoints.at(nTriangle*3).x
//                          << " v1 y: " <<trianglePoints.at(nTriangle*3).y
//                          << " v1 z: " <<trianglePoints.at(nTriangle*3).z
//                          << "   v2 x: " <<trianglePoints.at(nTriangle*3+1).x
//                          << " v2 y: " <<trianglePoints.at(nTriangle*3+1).y
//                          << " v2 z: " <<trianglePoints.at(nTriangle*3+1).z
//                          << "   v3 x: " <<trianglePoints.at(nTriangle*3+2).x
//                          << " v3 y: " <<trianglePoints.at(nTriangle*3+2).y
//                          << " v3 z: " <<trianglePoints.at(nTriangle*3+2).z
//                          << "  Target: x:"<< mTargetCoordinates.x << " y: " << mTargetCoordinates.y << " Z: " << mTargetCoordinates.z
//                          <<std::endl;
            }
        }

    }

    if(foundTriangle ==true)
    {
        //                std::cout << "Triangle found, Nr: " << nTriangle
        //                          << " v1 x: " <<trianglePoints.at(nTriangle*3).x
        //                          << " v1 y: " <<trianglePoints.at(nTriangle*3).y
        //                          << " v1 z: " <<trianglePoints.at(nTriangle*3).z
        //                          << "   v2 x: " <<trianglePoints.at(nTriangle*3+1).x
        //                          << " v2 y: " <<trianglePoints.at(nTriangle*3+1).y
        //                          << " v2 z: " <<trianglePoints.at(nTriangle*3+1).z
        //                          << "   v3 x: " <<trianglePoints.at(nTriangle*3+2).x
        //                          << " v3 y: " <<trianglePoints.at(nTriangle*3+2).y
        //                          << " v3 z: " <<trianglePoints.at(nTriangle*3+2).z
        //                          << "  Target: x:"<< mTargetCoordinates.x << " y: " << mTargetCoordinates.y << " Z: " << mTargetCoordinates.z
        //                          <<std::endl;
        //Found the triangle
        height = calcHeight(barCenCoor,p1,p2,p3);
        normal = calcNormal(barCenCoor,p1,p2,p3);
        calcAlpha();

        isOnTriangle = true;
        //       std::cout << "Normal x: "<< normal.x << " y: " << normal.y << " z: " << normal.z << std::endl;
        foundTriangle = false;
        numOfRounds = 0;

    }
    else
    {
       // std::cout << "Not within the area" << std::endl;
        height =1;
    }
}

void Gravity::checkBarycentricCoordinatesNoNeighbours(gsl::Vector3D mTargetCoordinates)
{
   // setTarget(mTargetCoordinates);
    gsl::Vector3D barCenCoor;
    gsl::Vector3D p1;
    gsl::Vector3D p2;
    gsl::Vector3D p3;

    bool foundTriangle = false;
    gsl::Vector2D target(mTargetCoordinates.x,mTargetCoordinates.z);

    for(int i = 0; i < trianglePoints.size()/3; i++)
    {

        p1 = trianglePoints.at(i);
        p2 = trianglePoints.at(i+1);
        p3 = trianglePoints.at(i+2);
        barCenCoor = gsl::Vector2D().barycentricCoordinates(gsl::Vector2D(p1.x,p1.z),gsl::Vector2D(p2.x,p2.z),gsl::Vector2D(p3.x,p3.z));
        if(barCenCoor.x > 0 && barCenCoor.y > 0 && barCenCoor.z > 0)
        {
            foundTriangle = true;
            //Found the triangle
            height = calcHeight(barCenCoor,p1,p2,p3);
            normal = calcNormal(barCenCoor,p1,p2,p3);
            calcAlpha();
            break;

        }
        else
        {
          //  std::cout << "Not within the area of triangles!" << std::endl;

            foundTriangle = false;
        }
    }
}




gsl::Vector3D Gravity::calcNormalForce()
{
    normalForce = normal*calcGravityForce().y * static_cast<float>(std::cos(alpha));//gsl::Vector3D().cross(normal,calcGravityForce())*static_cast<float>(std::cos (alpha));
    return normalForce;

    //normalForce = (v1-v0)x(v2-v0)
}

gsl::Vector3D Gravity::calcGravityForce()
{
    gravityForce = gsl::Vector3D(0,/*-1**/(gsl::GRAVITY*mass),0); // TODO: Hva er opp i prosjektet?
    return gravityForce;
}

float Gravity::calcHeight(gsl::Vector3D baryCoor, gsl::Vector3D p1, gsl::Vector3D p2, gsl::Vector3D p3)
{
    float temp;

    temp = (p1.y*baryCoor.x + p2.y *baryCoor.y + p3.y*baryCoor.z);

    return temp;
}
//Dette er normalen til triangelet,er det dette som er riktig normal?
gsl::Vector3D Gravity::calcNormal(gsl::Vector3D baryCoor,gsl::Vector3D p1, gsl::Vector3D p2, gsl::Vector3D p3)
{
    gsl::Vector3D v = p2-p1;
    gsl::Vector3D w = p3-p1;
    float Nx= (v.y*w.z)-(v.z*w.y);
    float Ny = (v.z*w.x)-(v.x*w.z);
    float Nz = (v.x*w.y)-(v.y*w.x);
    gsl::Vector3D temp(Nx, Ny, Nz);
    temp.normalize();

    return temp;
}

float Gravity::getHeight() const
{
    return height;
}

void Gravity::setRadius(float value)
{
    radius = value;
}

gsl::Vector3D Gravity::getAcceleration() const
{
    return acceleration;
}

void Gravity::setTriangles(std::vector<Vertex> vertices, std::vector<gsl::Vector3D> mNeighbours)
{
    for(auto vert : vertices)
    {
        trianglePoints.push_back(vert.get_XYZ());
    }
    std::cout <<trianglePoints.size() << std::endl;
    neighbours = mNeighbours;
}

void Gravity::calcAlpha()
{
    //n*k = cos alpha
    alpha = std::acos(static_cast<double>(gsl::Vector3D().dot(normal,gsl::Vector3D(0,1,0))));
}
