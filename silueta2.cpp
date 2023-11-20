#pragma once
#include "librerias.h"
#include <stdlib.h>
#include <time.h>

map<int, pair<vector<float>,int>> nuevocentroide;
map<string, map<string, float>> DISTANCES;
map<string, pair<int, float>> CentroidesCercanos;

string indiceVector(const vector<float>& indice) {
    string result;
    transform(indice.begin(), indice.end(), back_inserter(result), [](float i) { return static_cast<char>(i); });
    return result;
}

float distanciaEuclidiana(vector<float> centro, vector<float> dato){
    if (DISTANCES[indiceVector(centro)][indiceVector(dato)] == 0) {
        if(DISTANCES[indiceVector(dato)][indiceVector(centro)] == 0){
            int suma = 0;
            for(int i = 0; i < dato.size(); i++){
                suma += pow((centro[i] - dato[i]),2);
            }
            DISTANCES[indiceVector(centro)][indiceVector(dato)] = (float) sqrt(suma);
            return (float) sqrt(suma);
        }
        return DISTANCES[indiceVector(dato)][indiceVector(centro)];
    }
    return DISTANCES[indiceVector(centro)][indiceVector(dato)];
}

vector<vector<float>> centroide(vector<vector<float>> datos){
    srand (time(NULL));
    vector<vector<float>> result(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        result[i] = datos[rand() % datos.size()];
    }
    return result;
}

vector<float> nuevoCentroide(vector<float> p, int cluster){
    auto it = nuevocentroide.find(cluster);
    if (it != nuevocentroide.end()) {
        for(int i = 0; i < p.size(); i++){
            nuevocentroide[cluster].first[i] *= nuevocentroide[cluster].second;
            nuevocentroide[cluster].first[i] += p[i];
            nuevocentroide[cluster].first[i] /= nuevocentroide[cluster].second+1;
        }
        nuevocentroide[cluster].second++;
        return nuevocentroide[cluster].first;
    }
    nuevocentroide[cluster] = make_pair(p, 1);
    return p;
}

float hallarA(vector<float> p, vector<vector<float>> cluster){
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

float hallarB(vector<float> p, vector<vector<float>> centroides, vector<vector<vector<float>>> conjunto, vector<float> cluscentro){
    float indice = CentroidesCercanos[indiceVector(cluscentro)].first;
    cout << indice << endl;
    float bProm = 0;
    /*for(int i = 0; i < maxClusters; i++){
        if(cluscentro == centroides[i]) continue;
        float nuevaDistancia = distanciaEuclidiana(cluscentro, centroides[i]);
        if(nuevaDistancia < distanciaMenor){
            distanciaMenor = nuevaDistancia;
            indice = i;
        }
    }*/
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

float hallarSC(vector<vector<float>> Cluster,vector<vector<float>> Centroides, vector<vector<vector<float>>> Conjunto, vector<float> clusCentro){
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

int silueta(vector<vector<float>> datos){
    //Vectores a usar
    vector<vector<float>> centroides = centroide(datos);
    vector<vector<vector<float>>> Conjunto(maxClusters);
    vector<float> SC(maxClusters);

    //Ubicar puntos en el cluster correcto
    for(int i = 0; i < datos.size(); i++){
        float distancia_menor = 1e9;
        int cluster = 0;
        for(int j = 0; j < maxClusters; j++){
            float nueva_distancia = distanciaEuclidiana(centroides[j], datos[i]);
            if(nueva_distancia < distancia_menor){
                distancia_menor = nueva_distancia;
                cluster = j;
            }
        }
        Conjunto[cluster].push_back(datos[i]);
        centroides[cluster] = nuevoCentroide(datos[i],cluster);
    }
    for(int i = 0; i < maxClusters; i++){
        cout << i << endl;
        CentroidesCercanos[indiceVector(centroides[i])].first = -1;
        CentroidesCercanos[indiceVector(centroides[i])].second = 1e9;
        for(int j = 0; j < maxClusters; j++){
            if(centroides[i] == centroides[j]) continue;
            float nuevaDistancia = distanciaEuclidiana(centroides[i], centroides[j]);
            if(nuevaDistancia < CentroidesCercanos[indiceVector(centroides[i])].second){
                CentroidesCercanos[indiceVector(centroides[i])].first = j;
                CentroidesCercanos[indiceVector(centroides[i])].second = nuevaDistancia;
            }
        }
    }

    //Hallar SC
    for(int i = 0; i < maxClusters; i++){
        SC[i] = hallarSC(Conjunto[i],centroides, Conjunto, centroides[i]);
        cout << SC[i] << endl;
    }
    float masCerca = 1e9;
    int indice = 0;
    for(int i = 0; i < maxClusters; i++){
        if(abs(1-SC[i]) < masCerca){
            masCerca = abs(1-SC[i]);
            indice = i+1;
        }
    }

    for(int i = 0; i < SC.size(); i++){
        cout << i + 1 << " " << SC[i] << endl;
    }
    return indice;
}