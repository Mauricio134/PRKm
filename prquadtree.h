#pragma once
#include "librerias.h"

class Quadtree{
private:
    Point bottomLeft;
    float height;
    int nPoints;
    int nivel;
    Data * d;
    vector<Data *> conjunto;
    Quadtree * children[4];
public:
    Quadtree();
    Quadtree(const Point &, float &, int );
    ~Quadtree();
    void Insert(Data *);
    bool Outside(Data *);
    bool Overlap(float , float, float, float);
    void rangeQuery(float , float, float, float, set<Data *> &);
    bool Inside(float , float, float, float, Data * &);
    vector<Cluster *> Similarity(float , float, float, float , set<Data *> & );
};