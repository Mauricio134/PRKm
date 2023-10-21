#include <iostream>
#include "prquadtree.h"

using namespace std;

// Constructor de Clase Data sin argumentos.
Data::Data(){
    latitud = 0.00;
    longitud = 0.00;
    dato.resize(cantidad-2);
}

// Constructor de Clase Data con argumentos.
Data::Data(float lon, float lat, vector<float> a){
    latitud = lat;
    longitud = lon;
    dato = a;
}

// Constructor del Quadtree sin argumentos.
Quadtree::Quadtree(){
    d = nullptr;
    nPoints = 0;
}

//Constructir del Quadtree con argumentos.
Quadtree::Quadtree( const Point &nuevo , float &h){
    bottomLeft = nuevo;
    height = h;
    for(int i = 0; i < 4; i++) children[i] = new Quadtree();
}

// Funcion de Outside, para verificar si el punto no se sale del cuadrante.
bool Quadtree::Outside(const Data & p){
    return p.longitud < bottomLeft.x || p.longitud > bottomLeft.x + height || p.latitud < bottomLeft.y || p.latitud > bottomLeft.y + height;
}

// Funcion Insert
void Quadtree::Insert(const Data & p){
    if(Outside(p)) return;

    if(nPoints == 0 && d == nullptr){
        float altura = height/2.0;
        for(int i = 0; i < 4; i++){
            float childx = (i & 1) ? bottomLeft.x + altura : bottomLeft.x;
            float childy = (i & 2) ? bottomLeft.y + altura : bottomLeft.y;
            children[i] = new Quadtree(Point(childx,childy),altura);
        }
        d = new Data(p.longitud, p.latitud, p.dato);
        return;
    }
    else if(nPoints == 0 && d != nullptr){
        Data n_d = Data( d->longitud , d->latitud , d->dato );
        d = nullptr;
        float altura = height/2.0;
        int child = 0;
        if(n_d.longitud >= bottomLeft.x + altura) child |= 1;
        if(n_d.latitud >= bottomLeft.y + altura) child |= 2;
        children[child]->Insert(n_d);
    }
    float altura = height/2.0;
    int child = 0;
    if(p.longitud >= bottomLeft.x + altura) child |= 1;
    if(p.latitud >= bottomLeft.y + altura) child |= 2;
    children[child]->Insert(p);
    nPoints++;
}