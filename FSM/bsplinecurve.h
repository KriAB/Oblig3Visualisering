#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <vector>
#include "innpch.h"
//#include "gameobject.h"


class BSplineCurve //: public GameObject
{
public:
    // ikke alle disse funksjonene er nødvendige
    BSplineCurve();
    BSplineCurve(std::vector<float> knots, std::vector<gsl::Vector3D> controlpoints, int degree=2);
    void initVertexBufferObjects();
    void draw(GLint positionAttribute, GLint colorAttribute, GLint textureAttribute=-1);

    void setKnotsAndControlPoints( std::vector<gsl::Vector3D> controlPoints);
    gsl::Vector3D evaluateBSpline(int my, float x);
    float findKnotInterval(float x);
    gsl::Vector3D evaluateBSpline(int degree, int startKnot, float x);
    void makeKnots();
    gsl::Vector3D makePatrolPoint(float x, std::vector<gsl::Vector3D> controlpoints );
    int calcNumberOfKnots();
    int getMy(float x);

private:
    std::vector<gsl::Vector3D> b;      // control points
    int n;          //n = number of knots
    int d;          //d = degree
    std::vector<float> t;   // knots
};

#endif // BSPLINECURVE_H
