#ifndef SCENE_H
#define SCENE_H
#include "object.h"
#include <QWidget>

class Scene : public QWidget
{
    Q_OBJECT
    //RenderArea *renderArea;
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
public:
    Scene(QWidget *parent = 0);
    Dot light;
    Dot observer;
    Object object;
    vector<Plane> shadow;
    Plane table;
    void rotation(double alhpaX, double alphaY, double alphaZ);
    vector<vector<double>> getInverseMat(vector<vector<double>> A);
    Dot intersection(Dot a, Dot b, Plane plane);
    void defineShadow();
    void defineVisiblePlanes();
    ~Scene();
};

#endif // SCENE_H
