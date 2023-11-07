#include "librerias.h"

vector<vector<float>> centroide(vector<vector<float>> datos){
    vector<vector<float>> result(maxClusters);
    for(int i = 0; i < maxClusters; i++){
        result[i] = datos[rand() % datos.size()];
    }
    return result;
}

float distanciaEuclidiana(vector<float> centro, vector<float> dato){
    int suma = 0;
    for(int i = 0; i < dato.size(); i++){
        suma += pow((centro[i] - dato[i]),2);
    }
    return (float) sqrt(suma);
}

vector<float> nuevoCentroide(vector<vector<float>> conjunto){
    vector<float> nuevoCentro;
    int filas = conjunto.size();
    int columnas = conjunto[0].size();
    for(int i = 0; i < columnas; i++){
        float suma_indice = 0;
        for(int j = 0; j < filas; j++){
            suma_indice += conjunto[j][i];
        }
        nuevoCentro.push_back(suma_indice/filas);
    }
    return nuevoCentro;
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
    float distanciaMenor = 1e9;
    float indice = 0;
    float bProm = 0;
    for(int i = 0; i < maxClusters; i++){
        if(cluscentro == centroides[i]) continue;
        float nuevaDistancia = distanciaEuclidiana(cluscentro, centroides[i]);
        if(nuevaDistancia < distanciaMenor){
            distanciaMenor = nuevaDistancia;
            indice = i;
        }
    }
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
        centroides[cluster] = nuevoCentroide(Conjunto[cluster]);
    }

    //Hallar SC
    for(int i = 0; i < maxClusters; i++){
        SC[i] = hallarSC(Conjunto[i],centroides, Conjunto, centroides[i]);
    }
    float masCerca = 1e9;
    int indice = 0;
    for(int i = 0; i < maxClusters; i++){
        if(abs(1-SC[i]) < masCerca){
            masCerca = abs(1-SC[i]);
            indice = i+1;
        }
    }
    return indice;
}