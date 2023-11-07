#pragma once
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