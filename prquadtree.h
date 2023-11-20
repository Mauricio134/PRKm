#pragma once
#include "librerias.h"
#include "kmean.h"

class Quadtree{
private:
    Point bottomLeft;
    float height;
    int nPoints;
    int nivel;
    Data * d;
    vector<Data *> conjunto;
    Quadtree * children[4];
    KmeanTree * similitud;
public:
    Quadtree();
    Quadtree(const Point &, float &, int );
    void Insert(Data *);
    bool Outside(Data *);
    void Union(KmeanTree);
};