#ifndef PLANE_H
#define PLANE_H
#include "straight.h"


class Plane
{
public:
    vector<int> planeEq;
    vector<Straight> planeVec;
    Plane(){}
    Plane(Straight left, Straight right, Straight top, Straight bottom);
};

#endif // PLANE_H
