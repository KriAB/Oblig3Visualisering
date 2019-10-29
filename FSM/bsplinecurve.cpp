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
