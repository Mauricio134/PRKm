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
bool Quadtree::Outside(Data * p){
    return p->longitud < bottomLeft.x || p->longitud > bottomLeft.x + height || p->latitud < bottomLeft.y || p->latitud > bottomLeft.y + height;
}

// Funcion Insert
void Quadtree::Insert(Data * p){

    if(Outside(p)) return;

    if(nPoints == 0){
        float altura = height/2.0;
        for(int i = 0; i < 4; i++){
            float childx = (i & 1) ? bottomLeft.x + altura : bottomLeft.x;
            float childy = (i & 2) ? bottomLeft.y + altura : bottomLeft.y;
            children[i] = new Quadtree(Point(childx,childy),altura, nivel + 1);
        }
        d = p;
        nPoints++;
        conjunto.push_back(p);
        return;
    }
    else if(d != nullptr){
        if(nivel >= mxAltura){
            d = new Data(p->longitud, p->latitud, p->dato);
            nPoints++;
            conjunto.push_back(p);
            return;
        }
        Data * n_d = d;
        d = nullptr;
        float altura = height/2.0;
        int child = 0;
        if(n_d->longitud >= bottomLeft.x + altura) child |= 1;
        if(n_d->latitud >= bottomLeft.y + altura) child |= 2;
        children[child]->Insert(n_d);
    }
    float altura = height/2.0;
    int child = 0;
    if(p->longitud >= bottomLeft.x + altura) child |= 1;
    if(p->latitud >= bottomLeft.y + altura) child |= 2;
    children[child]->Insert(p);
    nPoints++;
    conjunto.push_back(p);
}

bool Quadtree::Overlap(const Point & init, const float & h){
    int c_dif_x = min(init.x + h, bottomLeft.x + height) - max(init.x, bottomLeft.x);
    int c_dif_y = min(init.y + h, bottomLeft.y + height) - max(init.y, bottomLeft.y);
    if(c_dif_x > 0 && c_dif_y > 0) return true;
    return false;
}

bool Quadtree::Inside(const Point &init , const float &h, Data * &p){
    if(p->longitud < init.x || p->longitud > init.x + h || p->latitud < init.y || p->latitud > init.y + h) return false;
    return true;
}

void Quadtree::rangeQuery(const Point & init, const float & h, set<Data *> & result){
    if(!Overlap(init, h)) return;

    if(d != nullptr || nivel >= mxAltura){
        for(int i = 0; i < conjunto.size(); i++){
            if(!Inside(init, h, conjunto[i]) || result.count(conjunto[i])) continue;
            result.insert(conjunto[i]);
        }
        return;
    }
    for(int i = 0; i < 4; i++){
        children[i]->rangeQuery(init, h, result);
    }
    return;
}

set<Data *> Quadtree::Similarity(const Point & init, const float & h, set<Data *> & group){
    set<Data *> resultado;
    for(auto it = group.begin(); it != group.end(); it++){
        for(int i = 0; i < (*it)->similitud->clusters.size(); i++){
            for(int j = 0; j < (*it)->similitud->clusters[i]->Set.size(); j++){
                if(!Inside(init, h, (*it)->similitud->clusters[i]->Set[j]) || resultado.count((*it)->similitud->clusters[i]->Set[j])) continue;
                resultado.insert((*it)->similitud->clusters[i]->Set[j]);
            }
        }
    }
    return resultado;
}