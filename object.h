#ifndef OBJECT_H
#define OBJECT_H
#include "plane.h"

class Object
{
public:
    mat objMat;
    bool screenedPlanes[6];
    bool visiblePlanes[6]={true,true,true,true,true,true};
    vector<Plane> planes;
    Object(vector<Plane> planes);
    Object(){}
};

#endif // OBJECT_H
