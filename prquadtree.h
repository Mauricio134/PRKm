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
    void Insert(Data *);
    bool Outside(Data *);
    bool Overlap(const Point &, const float &);
    void rangeQuery(const Point &, const float &, set<Data *> &);
    bool Inside(const Point &, const float &, Data * &);
    set<Data *> Similarity(const Point &, const float & , set<Data *> & );
};