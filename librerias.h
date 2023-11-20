#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <map>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

#define dimension 32
#define maxClusters 10

#define mxAltura 3

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
    int iterador;
    Data();
    Data(float, float, vector<float>);
};

Data::Data(){
    latitud = 0.00;
    longitud = 0.00;
    dato.resize(dimension-2);
}

// Constructor de Clase Data con argumentos.
Data::Data(float lon, float lat, vector<float> a){
    latitud = lat;
    longitud = lon;
    dato = a;
}