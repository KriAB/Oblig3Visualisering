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

    void setKnotsAndControlPoints(std::vector<float> knots, std::vector<gsl::Vector3D> points);
    gsl::Vector3D evaluateBSpline(int my, float x);
    int findKnotInterval(float x);
    gsl::Vector3D evaluateBSpline(int degree, int startKnot, float x);


private:
    std::vector<gsl::Vector3D> b;      // control points
    int n;          //n = number of knots
    int d;          //d = degree
    std::vector<float> t;   // knots
};

#endif // BSPLINECURVE_H
