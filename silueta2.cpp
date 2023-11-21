#pragma once
#include "librerias.h"
#include <stdlib.h>
#include <time.h>

map<int, pair<Data *,int>> nuevocentroide;
map<Data *, map<Data *, float>> DISTANCES;
map<Data *, pair<int, float>> CentroidesCercanos;

string indiceVector(const vector<float>& indice) {
    string result;
    transform(indice.begin(), indice.end(), back_inserter(result), [](float i) { return static_cast<char>(i); });
    return result;
}

float distanciaEuclidiana(Data * d1, Data * d2){
    if (DISTANCES[d1][d2] == 0) {
        if(DISTANCES[d2][d1] == 0){
            int suma = 0;
            for(int i = 0; i < d1->dato.size(); i++){
                suma += pow((d1->dato[i] - d2->dato[i]),2);
            }
            DISTANCES[d1][d2] = (float) sqrt(suma);
            return (float) sqrt(suma);
        }
        return DISTANCES[d2][d1];
    }
    return DISTANCES[d1][d2];
}

vector<Data *> centroide(vector<Data> datos){
    srand(time(NULL));
    vector<Data *> result(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        result[i] = &(datos[rand() % datos.size()]);
    }
    return result;
}

Data * nuevoCentroide(Data * p, int cluster){
    auto it = nuevocentroide.find(cluster);
    if (it != nuevocentroide.end()) {
        for(int i = 0; i < p->dato.size(); i++){
            nuevocentroide[cluster].first->dato[i] *= nuevocentroide[cluster].second;
            nuevocentroide[cluster].first->dato[i] += p->dato[i];
            nuevocentroide[cluster].first->dato[i] /= nuevocentroide[cluster].second+1;
        }
        nuevocentroide[cluster].second++;
        return nuevocentroide[cluster].first;
    }
    nuevocentroide[cluster] = make_pair(p, 1);
    return p;
}

float hallarA(Data * p, vector<Data *> cluster){
    float aProm = 0;
    for(int i = 0; i < cluster.size(); i++){
        if(p == cluster[i]) continue;
        aProm += distanciaEuclidiana(cluster[i], p);
    }
    if(cluster.size() == 1 || cluster.size() == 0){
        return 0;
    }
    return (float)(aProm/(cluster.size()-1));
}

float hallarB(Data * p, vector<Data *> centroides, vector<vector<Data *>> conjunto, Data * cluscentro){
    float indice = CentroidesCercanos[cluscentro].first;
    float bProm = 0;
    for(int i = 0; i < conjunto[indice].size(); i++){
        if(p == conjunto[indice][i]){ continue; }
        bProm += distanciaEuclidiana(conjunto[indice][i], p);
    }
    if(conjunto[indice].size() != 0) return bProm/conjunto[indice].size();
    else return 0;
}

float SC(float a, float b){
    if(a == b){
        return (float) 0;
    }
    else if(a < b){
        return (float) (1-a/b);
    }
    else{
        return (float) (b/a - 1);
    }
}

float hallarSC(vector<Data *> Cluster,vector<Data *> Centroides, vector<vector<Data *>> Conjunto, Data * clusCentro){
    float scPromedio = 0;
    for(int i = 0; i < Cluster.size(); i++){
        float a = hallarA(Cluster[i], Cluster);
        float b = hallarB(Cluster[i], Centroides, Conjunto, clusCentro);
        scPromedio += SC(a,b);
    }
    if(Cluster.size() == 0) return 0;
    scPromedio /= Cluster.size();
    return (float)scPromedio;
}

int silueta(vector<Data> datos){
    //Vectores a usar
    vector<Data *> centroides = centroide(datos);
    vector<vector<Data *>> Conjunto(maxClusters);
    vector<float> SC(maxClusters);

    //Ubicar puntos en el cluster correcto
    for(int i = 0; i < datos.size(); i++){
        float distancia_menor = 1e9;
        int cluster = 0;
        for(int j = 0; j < maxClusters; j++){
            float nueva_distancia = distanciaEuclidiana(centroides[j], &datos[i]);
            if(nueva_distancia < distancia_menor){
                distancia_menor = nueva_distancia;
                cluster = j;
            }
        }
        Conjunto[cluster].push_back(&datos[i]);
        centroides[cluster] = nuevoCentroide(&datos[i],cluster);
    }
    for(int i = 0; i < maxClusters; i++){
        CentroidesCercanos[centroides[i]].first = -1;
        CentroidesCercanos[centroides[i]].second = 1e9;
        for(int j = 0; j < maxClusters; j++){
            if(centroides[i] == centroides[j]) continue;
            float nuevaDistancia = distanciaEuclidiana(centroides[i], centroides[j]);
            if(nuevaDistancia < CentroidesCercanos[centroides[i]].second){
                CentroidesCercanos[centroides[i]].first = j;
                CentroidesCercanos[centroides[i]].second = nuevaDistancia;
            }
        }
    }

    //Hallar SC
    for(int i = 0; i < maxClusters; i++){
        SC[i] = hallarSC(Conjunto[i],centroides, Conjunto, centroides[i]);
    }
    float masCerca = 1e9;
    int indice = 0;
    for(int i = 0; i < maxClusters; i++){
        if(abs(1-SC[i]) <= masCerca){
            masCerca = abs(1-SC[i]);
            indice = i+1;
        }
    }

    for(int i = 0; i < SC.size(); i++){
        cout << i + 1 << " " << SC[i] << endl;
    }
    return indice;
}