#include "scene.h"
#include "QPainter"
#define OFFSET 300
#define OBJECT_SIZE 100
Scene::Scene(QWidget *parent)
    : QWidget(parent)
{
    this->resize(QSize(1300,700));

    light.coords={-50,200,-250,1};
    observer.coords={100,100,100000,1};
    table.planeEq={0,1,0,0};

    //lower plane
    Dot tdot1(0, 0, 0);
    Dot tdot2(OBJECT_SIZE, 0, 0);
    Dot tdot3(0, 0, OBJECT_SIZE);
    Dot tdot4(OBJECT_SIZE, 0, OBJECT_SIZE);

    //higher plane
    Dot tdot5(0, OBJECT_SIZE, 0);
    Dot tdot6(OBJECT_SIZE, OBJECT_SIZE, 0);
    Dot tdot7(0, OBJECT_SIZE, OBJECT_SIZE);
    Dot tdot8(OBJECT_SIZE, OBJECT_SIZE, OBJECT_SIZE);

    object.planes.push_back(Plane(Straight(tdot1,tdot2), Straight(tdot2, tdot4), Straight(tdot4, tdot3), Straight(tdot3, tdot1)));
    object.planes.push_back(Plane(Straight(tdot1,tdot2), Straight(tdot2, tdot6), Straight(tdot6, tdot5), Straight(tdot5, tdot1)));
    object.planes.push_back(Plane(Straight(tdot1,tdot3), Straight(tdot3, tdot7), Straight(tdot7, tdot5), Straight(tdot5, tdot1)));
    object.planes.push_back(Plane(Straight(tdot2,tdot4), Straight(tdot4, tdot8), Straight(tdot8, tdot6), Straight(tdot6, tdot2)));
    object.planes.push_back(Plane(Straight(tdot3,tdot4), Straight(tdot4, tdot8), Straight(tdot8, tdot7), Straight(tdot7, tdot3)));
    object.planes.push_back(Plane(Straight(tdot5,tdot6), Straight(tdot6, tdot8), Straight(tdot8, tdot7), Straight(tdot7, tdot5)));

//    object.objMat={{0,1,0,0},{0,0,1,0},{1,0,0,0},{1,0,0,OBJECT_SIZE},{0,0,1,OBJECT_SIZE},{0,1,0,OBJECT_SIZE}};
    object.objMat={{0,0,-1,1,0,0},{-1,0,0,0,0,1},{0,-1,0,0,1,0},{0,0,0,-OBJECT_SIZE,-OBJECT_SIZE,-OBJECT_SIZE}};

    defineShadow();
    rotation(15,20,0);
    defineVisiblePlanes();
}

void Scene::paintEvent(QPaintEvent * /* event */){
    QPainter painter(this);
    painter.translate(400,400);
    painter.rotate(180);
    painter.setRenderHint(QPainter::Antialiasing, true);


    //Initializing & drawing object polygons

    QPoint points[4];
    int i=0;

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    for(Plane plane:shadow){
        for(Straight straight:plane.planeVec){
            points[i++]=QPoint(-straight.b.coords[0],straight.b.coords[1]);
        }
        painter.drawPolygon(points,4);
        i=0;
    }

    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    int j=0;
    for(Plane plane:object.planes){
        for(Straight straight:plane.planeVec){

            points[i++]=QPoint(-straight.b.coords[0],straight.b.coords[1]);
        }
        if(object.visiblePlanes[j]){
          if(object.screenedPlanes[j]){
              painter.setPen(Qt::black);
              painter.setBrush(Qt::black);
          } else {
              painter.setPen(Qt::blue);
              painter.setBrush(Qt::blue);
          }
          painter.drawPolygon(points,4);
        }
        i=0;
        j++;
    }


    //drawing light dot
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPoint(-light.coords[0],light.coords[1]),10-light.coords[2]*0.01,10-light.coords[2]*0.01);



}



Scene::~Scene(){

}

void Scene::rotation(double alphaX, double alphaY, double alphaZ){

    //Translate into radians
    alphaX=alphaX * PI / 180;
    alphaY=alphaY * PI / 180;
    alphaZ=alphaZ * PI / 180;

    //X rotation Matrix
    mat rotX={{1,0,0,0},
             {0,cos(alphaX),sin(alphaX),0},
             {0,-sin(alphaX),cos(alphaX),0},
             {0,0,0,1}};
    //Y rotation Matrix
    mat rotY={{cos(alphaY),0,sin(alphaY),0},
              {0,1,0,0},
              {-sin(alphaY),0,cos(alphaY),0},
              {0,0,0,1}};
    //Z rotation Matrix
    mat rotZ={{cos(alphaZ),-sin(alphaZ),0,0},
              {sin(alphaZ),cos(alphaZ),0,0},
              {0,0,1,0},
              {0,0,0,1}};


    //X rotation
    for(Plane &plane:object.planes){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotX*straight.a.coords;
            straight.b.coords=rotX*straight.b.coords;
        }
    }

    for(Plane &plane:shadow){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotX*straight.a.coords;
            straight.b.coords=rotX*straight.b.coords;
        }
    }
    //Y rotation
    for(Plane &plane:object.planes){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotY*straight.a.coords;
            straight.b.coords=rotY*straight.b.coords;
        }
    }

    for(Plane &plane:shadow){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotY*straight.a.coords;
            straight.b.coords=rotY*straight.b.coords;
        }
    }

    //Z rotation
    for(Plane &plane:object.planes){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotZ*straight.a.coords;
            straight.b.coords=rotZ*straight.b.coords;

        }
    }
    for(Plane &plane:shadow){
        for(Straight &straight:plane.planeVec){
            straight.a.coords=rotZ*straight.a.coords;
            straight.b.coords=rotZ*straight.b.coords;

        }
    }


    //rotating the object matrix
    object.objMat=-rotX*object.objMat;
    object.objMat=-rotY*object.objMat;
    object.objMat=-rotZ*object.objMat;

    //rotating the light dot
//    light.coords=light.coords.t();
    light.coords=rotX*light.coords;
    light.coords=rotY*light.coords;
    light.coords=rotZ*light.coords;


}





vector<vector<double>> Scene::getInverseMat(vector<vector<double> > A){
    Mat<double> Aarm(A.size(), A[0].size());
    Mat<double> Barm(A.size(), A[0].size());
    for(int i=0;i<A.size();i++){
        for(int j=0;j<A[0].size();j++){
            Aarm(i,j)=A[i][j];
        }
    }
    Barm=inv(Aarm);

    return A;
}

Dot Scene::intersection(Dot a,Dot b, Plane plane){
    double x1=a.coords[0];
    double x2=b.coords[0];
    double y1=a.coords[1];
    double y2=b.coords[1];
    double z1=a.coords[2];
    double z2=b.coords[2];

    double A=plane.planeEq[0];
    double B=plane.planeEq[1];
    double C=plane.planeEq[2];
    double D=plane.planeEq[3];

    double alpha=-(A*x1+B*y1+C*z1+D)/(A*(x2-x1)+B*(y2-y1)+C*(z2-z1));

    return Dot(x1+alpha*(x2-x1),y1+alpha*(y2-y1),z1+alpha*(z2-z1));
}



void Scene::defineShadow(){
    Dot projectionA;
    Dot projectionB;
    for(Plane plane:object.planes){
        shadow.push_back(Plane());
        for(Straight straight:plane.planeVec){
            projectionA=intersection(straight.a, light, table);
            projectionB=intersection(straight.b, light, table);
            shadow.back().planeVec.push_back(Straight(projectionA, projectionB));
        }
    }

    rowvec coords=light.coords.t();
    rowvec isScreened=coords*object.objMat;
    for(int i=0;i<6;i++){
        if(isScreened[i]<0)
            object.screenedPlanes[i]=true;
        else
            object.screenedPlanes[i]=false;
    }

}


void Scene::defineVisiblePlanes(){
    rowvec obsCoords=observer.coords.t();
    rowvec isVisible=obsCoords*object.objMat;
    for(int i=0;i<6;i++){
        if(isVisible[i]<=0)
            object.visiblePlanes[i]=false;
        else
            object.visiblePlanes[i]=true;
    }
}

















