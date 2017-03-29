#include "plane.h"

Plane::Plane(Straight left, Straight right, Straight top, Straight bottom){
    planeVec.push_back(left);
    planeVec.push_back(right);
    planeVec.push_back(top);
    planeVec.push_back(bottom);
}
