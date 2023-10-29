#include "librerias.h"
#include "kmean.h"

vector<vector<float>> centers(vector<vector<float>> &datos){
    vector<vector<float>> centroides(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        centroides[i] = datos[rand() % datos.size()];
    }
    return centroides;
}

float distancia2(vector<float> &centroide, vector<float> &dato){
    float suma = 0;
    for(int i = 0; i < dato.size(); i++){
        suma += pow((centroide[i]-dato[i]),2);
    }

    return sqrt(suma);
}

float promDist(vector<float> &distancias){
    if(distancias.empty()) return 0;
    float suma = 0;
    for(int i = 0; i < distancias.size(); i++){
        suma += distancias[i];
    }
    return suma/distancias.size();
}

int silueta(vector<vector<float>> &puntos){
    //freopen("output.txt", "w", stdout);
    vector<vector<float>> centroides = centers(puntos);
    vector<vector<float>> dist(maxClusters);
    vector<vector<vector<float>>> datos(maxClusters);
    vector<float> a(maxClusters);
    vector<float> b(maxClusters);
    for(int i = 0; i < puntos.size(); i++){
        int k = 0;
        float distancia_inicial = distancia2(centroides[0], puntos[i]);
        for(int c = 1; c < maxClusters; c++){
            float nueva_distancia = distancia2(centroides[c], puntos[i]);
            if(nueva_distancia < distancia_inicial){
                k = c;
                distancia_inicial = nueva_distancia;
            }
        }
        dist[k].push_back(distancia_inicial);
        datos[k].push_back(puntos[i]);
    }
    for(int i = 0; i < maxClusters; i++){
        a[i] = promDist(dist[i]);
    }
    vector<vector<float>> nc = centroides;
    vector<vector<float>> bdist(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        int k = 0;
        float distancia_inicial = 1e6;
        for(int j = 0; j < maxClusters; j++){
            if(nc[i] == centroides[j]) continue;
            int nueva_distancia = distancia2(nc[i], centroides[j]);
            if(nueva_distancia < distancia_inicial){
                distancia_inicial = nueva_distancia;
                k = j;
            }
        }
        for(int j = 0; j < datos[k].size(); j++){
            bdist[i].push_back(distancia2(centroides[k],datos[k][j]));
        }
    }
    for(int i = 0; i < maxClusters; i++){
        b[i] = promDist(bdist[i]);
    }
    vector<float> silo(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        if(a[i] == b[i]){
            silo[i] = 0;
        }else if( a[i] < b[i]){
            silo[i] = 1 - a[i]/b[i];
        }
        else{
            silo[i] = b[i]/a[i]-1;
        }
    }
    int response = 0;
    float distancia_1 = abs(1-silo[0]);
    for(int i = 1; i < maxClusters; i++){
        int distancia_2 = abs(1-silo[i]);
        if(distancia_2 < distancia_1){
            distancia_1 = distancia_2;
            response = i+1;
        }
    }
    return response;
}