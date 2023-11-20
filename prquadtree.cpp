#pragma once
#include "librerias.h"
#include "prquadtree.h"

// Constructor de Clase Data sin argumentos.

// Constructor del Quadtree sin argumentos.
Quadtree::Quadtree(){
    d = nullptr;
    nPoints = 0;
}

//Constructir del Quadtree con argumentos.
Quadtree::Quadtree( const Point &nuevo , float &h, int n){
    bottomLeft = nuevo;
    height = h;
    nivel = n;
    nPoints = 0;
    for(int i = 0; i < 4; i++) children[i] = new Quadtree();
}

// Funcion de Outside, para verificar si el punto no se sale del cuadrante.
bool Quadtree::Outside(const Data & p){
    return p.longitud < bottomLeft.x || p.longitud > bottomLeft.x + height || p.latitud < bottomLeft.y || p.latitud > bottomLeft.y + height;
}

// Funcion Insert
void Quadtree::Insert(const Data & p){

    if(Outside(p)) return;

    if(nPoints == 0){
        float altura = height/2.0;
        for(int i = 0; i < 4; i++){
            float childx = (i & 1) ? bottomLeft.x + altura : bottomLeft.x;
            float childy = (i & 2) ? bottomLeft.y + altura : bottomLeft.y;
            children[i] = new Quadtree(Point(childx,childy),altura, nivel + 1);
        }
        d = new Data(p.longitud, p.latitud, p.dato);
        nPoints++;
        conjunto.push_back(p);
        return;
    }
    else if(d != nullptr){
        if(nivel >= mxAltura){
            d = new Data(p.longitud, p.latitud, p.dato);
            nPoints++;
            conjunto.push_back(p);
            return;
        }
        if(p.longitud == d->longitud && p.latitud == d->latitud) return;
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
    conjunto.push_back(p);
}

//Funcion Union
void Quadtree::Union(KmeanTree mean){
    for(int i = 0; i < 4; i++){
        if(d != nullptr){
            mean.clusterSelect(similitud, conjunto);
            return;
        }
        else if(nPoints == 0){
            return;
        }
        children[i]->Union(mean);
    }
    return;
}