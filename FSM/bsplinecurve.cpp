#include "bsplinecurve.h"
// Parametre inn:
// x - en parameterverdi på skjøtvektor
// my - et tall slik at t[my] <= x < t[my+1]
// Returverdi: et punkt på splinekurven
// b,n,d,t er objektvariable i klassen BSplineCurve
BSplineCurve::BSplineCurve()
{

}

BSplineCurve::BSplineCurve(std::vector<float> knots, std::vector<gsl::Vector3D> controlpoints, int degree)
{
    b = controlpoints;
    t = knots;
    n = b.size();
    d = degree;

}

void BSplineCurve::setKnotsAndControlPoints(std::vector<gsl::Vector3D> controlPoints)
{
    b = controlPoints;
    n = controlPoints.size();
    makeKnots();
}


void BSplineCurve::makeKnots()
{
    // d = degree
    // n = numer of controllPoints
    // t = knots
    // lengde av vektor for å finne knot
    // tre første = 0;
    //tre siste = 1;

    // n + 2
    //n + d (Maximum value of knot vector)

    //if(numberOfNots > 6)
    //De som er i midten er (1/((antall i midten)+1)))
    //else
    //1.halvdel = 0, 2.halvdel = 1
    t.clear();
    int numberOfKnots = calcNumberOfKnots();
    if(d == 1)
    {
        t.push_back(0);
        t.push_back(0);

        float interval = findKnotInterval(0);
        if(interval >= 0)
        {
            int temp = 1;

            //Vet om kontrollpunkter i midten
            int numInMiddle = numberOfKnots - 4;
            for(int i = 0; i < numInMiddle; i++)
            {
                t.push_back(temp * interval);
                temp++;
            }
            t.push_back(1);
            t.push_back(1);
        }
    }

    if(d == 2)
    {
        t.push_back(0);
        t.push_back(0);
        t.push_back(0);

        float interval = findKnotInterval(numberOfKnots);
        if(interval >= 0)
        {
            int temp = 1;

            //Vet om kontrollpunkter i midten
            int numInMiddle = numberOfKnots - 6;
            for(int i = 0; i < numInMiddle; i++)
            {
                t.push_back(temp * interval);
                temp++;
            }
            t.push_back(1);
            t.push_back(1);
            t.push_back(1);
        }
    }
}

gsl::Vector3D BSplineCurve::makePatrolPoint(float x, std::vector<gsl::Vector3D> controlpoints)
{
    setKnotsAndControlPoints(controlpoints);
    return evaluateBSpline(getMy(x), x);
}

int BSplineCurve::calcNumberOfKnots()
{
    int numberOfKnots;
    if(b.size() <= 2)
    {
        d = 1;
    }
    else
    {
        d = 2;
    }
    if(n >= d+1)
    {
        numberOfKnots = n + d + 1;
    }
}

int BSplineCurve::getMy(float x)
{
    // CALCULATE VALID KNOT INTERVAL 'MY'
    for (unsigned int my{0}; my < t.size() - 1; ++my)
        if (t[my] <= x && x < t[my + 1])
            return static_cast<int>(my);
    return -1;
}

gsl::Vector3D BSplineCurve::evaluateBSpline(int my, float x)
{
    gsl::Vector3D a[20]; // forutsetter da at n+d+1 <= 20
    for (int j=0; j<=d; j++)
        a[d-j] = b[my-j];

    for (int k=d; k>0; k--) {
        int j = my-k;
        for (int i=0; i<k; i++) {
            j++;
            float w = (x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a[0];
}

float BSplineCurve::findKnotInterval(int numberOfKnots)
{
    float numberInMiddle;
    if(d==2)
        numberInMiddle = numberOfKnots - 6;

    if(d==1)
        numberInMiddle = numberOfKnots - 4;
    return (1/ (numberInMiddle + 1));

}

gsl::Vector3D BSplineCurve::evaluateBSpline(int degree, int startKnot, float x)
{

}
