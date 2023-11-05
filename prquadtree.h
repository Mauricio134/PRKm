#pragma once
#include "librerias.h"

class Point{
public:
    float x;
    float y;
    Point() : x(0.00), y(0.00){};
    Point(float x_, float y_) : x(x_), y(y_){};
};

class Data{
public:
    float longitud;
    float latitud;
    vector<float> dato;
    Data();
    Data(float, float, vector<float>);
};

class Quadtree{
private:
    Point bottomLeft;
    float height;
    int nPoints;
    Data * d;
    Quadtree * children[4];
public:
    Quadtree();
    Quadtree(const Point &, float &);
    void Insert(const Data &);
    bool Outside(const Data &);
};