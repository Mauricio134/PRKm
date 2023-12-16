#pragma once
#include "librerias.h"
#include "kmean.h"

map<int, pair<Data *,int>> nueCentroide;

float distanciaEu(Data * d1, Data * d2){
    int suma = 0;
    for(int i = 0; i < d2->dato.size(); i++){
        suma += (d1->dato[i] - d2->dato[i])*(d1->dato[i] - d2->dato[i]);
    }
    return (float) sqrt(suma);
}

Data * nuevoCen(Data * p, int cluster){
    auto it = nueCentroide.find(cluster);
    if (it != nueCentroide.end()) {
        for(int i = 0; i < p->dato.size(); i++){
            nueCentroide[cluster].first->dato[i] *= nueCentroide[cluster].second;
            nueCentroide[cluster].first->dato[i] += p->dato[i];
            nueCentroide[cluster].first->dato[i] /= nueCentroide[cluster].second+1;
        }
        nueCentroide[cluster].second++;
        return nueCentroide[cluster].first;
    }
    nueCentroide[cluster] = make_pair(p, 1);
    return p;
}

Cluster::Cluster(){
    next = new KmeanTree();
}

Cluster::~Cluster(){
    for (Data *data : Set) {
        delete data;
    }
    delete valor;
    delete next;

}

KmeanTree::KmeanTree(){
    clusters.resize(divisiones);
}

KmeanTree::~KmeanTree(){
    for (Cluster *cluster : clusters) {
        delete cluster;
    }
}

KmeanTree::KmeanTree(int k, int nuevah){
    clusters.resize(k);
    divisiones = k;
    altura = nuevah;
    for(int i = 0; i < k; i++) clusters[i] = new Cluster();
}

void KmeanTree::Insert(vector<Data *> & datos, vector<Data *> & centroides, vector<vector<Data *>> &setcomplete){
    //seleccionar Centroides

    for(int i = 0; i < divisiones; i++){
        clusters[i]->valor = centroides[i];
        clusters[i]->H = altura;
    }

    for(int i = 0; i < setcomplete.size(); i++){
        for(int j = 0; j < setcomplete[i].size(); j++){
            setcomplete[i][j]->similitud = this->clusters[i];
            clusters[i]->Set.push_back(setcomplete[i][j]);
        }
    }
    if(divisiones == 1){
        return;
    }

    for(int i = 0; i < divisiones; i++){
        if(altura > mxAltura){
            return;
        }
        else if(clusters[i]->Set.size() == 1){
            continue;
        }
        pair<vector<vector<Data *>>,pair<vector<Data *>, int>> k = silueta3(clusters[i]->Set);
        if(k.second.second == 0) continue;
        clusters[i]->next = new KmeanTree(k.second.second, altura+1);
        clusters[i]->next->Insert(clusters[i]->Set, k.second.first, k.first);
    }
}