#ifndef DOT_H
#define DOT_H
#include <armadillo>
#include <cmath>
#include "vector"
#define PI 3.14159265
using namespace std;
using namespace arma;

class Dot
{

public:
    vec coords;
    Dot(double,double,double);
    Dot(){}
};

#endif // DOT_H
