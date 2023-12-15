#pragma once
#include "librerias.h"

vector<vector<float>> distancesPoint(17200, vector<float>(17200, 0.0f));
map<Data *, pair<vector<float>, int>> actualizarCentroide;
map<pair<Data *, Data *>, float> centroideApunto;

float distEuToCentroide(Data * &punto1, Data * &punto2){
    auto iterador = centroideApunto.find(make_pair(punto1, punto2));
    if (iterador != centroideApunto.end()){
        return iterador->second;
    }
    iterador = centroideApunto.find(make_pair(punto2, punto1));
    if (iterador != centroideApunto.end()){
        return iterador->second;
    }
    float resultado = 0.0f;
    for(int i = 0; i < punto1->dato.size(); i++){
        resultado += (punto1->dato[i]-punto2->dato[i])*(punto1->dato[i]-punto2->dato[i]);
    }
    centroideApunto[make_pair(punto1,punto2)] = sqrt(resultado);
    return centroideApunto[make_pair(punto1,punto2)];
}

void iniCentroides(vector<Data *> &datos, vector<Data *> & centroides){
    srand(time(NULL));
    int numRandom = rand() % datos.size();
    centroides[0] = new Data(datos[numRandom]->longitud, datos[numRandom]->latitud, datos[numRandom]->dato);
    for(int i = 1; i < centroides.size(); i++){
        vector<float> distCuadratica(datos.size(), 1e9f);
        for (int j = 0; j < datos.size(); ++j) {
            for (int c = 0; c < i; ++c) {
                float distancia = distEuToCentroide(datos[j], centroides[c]);
                distCuadratica[j] = min(distCuadratica[j], distancia);
            }
        }
        float totalDistSquared = 0.0f;
        for (float distSquared : distCuadratica) {
            totalDistSquared += distSquared;
        }

        // Choose the next centroid with probability proportional to the distance squared
        float randomValue = (float)rand() / RAND_MAX;
        float cumulativeProbability = 0.0f;
        for (int j = 0; j < datos.size(); ++j) {
            cumulativeProbability += distCuadratica[j] / totalDistSquared;
            if (randomValue <= cumulativeProbability) {
                centroides[i] = new Data(datos[j]->longitud, datos[j]->latitud, datos[j]->dato);
                break;
            }
        }
    }
}

void nuevoCentroide(Data * & centroide, vector<float> & punto){
    auto iterador = actualizarCentroide.find(centroide);
    if (iterador != actualizarCentroide.end()){
        for(int i = 0; i < centroide->dato.size(); i++){
            iterador->second.first[i] *= iterador->second.second;
            iterador->second.first[i] += punto[i];
            iterador->second.first[i] /= iterador->second.second+1;
        }
        iterador->second.second++;
    }
    else{
        actualizarCentroide[centroide] = make_pair(centroide->dato, 2);
        for(int i = 0; i < punto.size(); i++){
            actualizarCentroide[centroide].first[i] += punto[i];
            actualizarCentroide[centroide].first[i] /= 2;
        }
    }
    centroide->dato = actualizarCentroide[centroide].first;
}

float distEuToPoint(Data * punto1, Data * punto2){
    if(distancesPoint[punto1->etiqueta][punto2->etiqueta] != 0.0f){
        return distancesPoint[punto1->etiqueta][punto2->etiqueta];
    }
    else if(distancesPoint[punto2->etiqueta][punto1->etiqueta] != 0.0f){
        return distancesPoint[punto2->etiqueta][punto1->etiqueta];
    }
    float resultado = 0.0f;
    for(int i = 0; i < punto1->dato.size(); i++){
        resultado += (punto1->dato[i]-punto2->dato[i])*(punto1->dato[i]-punto2->dato[i]);
    }
    distancesPoint[punto1->etiqueta][punto2->etiqueta] = sqrt(resultado);
    return distancesPoint[punto1->etiqueta][punto2->etiqueta];
}

float a(vector<vector<Data *>> &Sets, Data * punto){
    int indice = punto->cluster;
    int tamanoSet = Sets[indice].size();
    float resultado = 0.0f;
    for(int i = 0; i < tamanoSet; i++){
        resultado += distEuToPoint(punto, Sets[indice][i]);
    }
    return resultado/(float) tamanoSet;
}

float b(vector<vector<Data *>> &Sets, Data * &punto, int k){
    int noIndice = punto->cluster;
    float valor = 1e9;
    for(int i = 0; i < k; i++){
        if(i == noIndice) continue;
        float resultado = 0.0f;
        int tamanoSeti = Sets[i].size();
        for(int j = 0; j < tamanoSeti; j++){
            resultado += distEuToPoint(punto, Sets[i][j]);
        }
        resultado /= (float) tamanoSeti;
        valor = min(valor, resultado);
    }
    return valor;
}

pair<vector<vector<Data *>>,pair<vector<Data *>, int>> silueta3(vector<Data *> &datos){
    float distTo1 = 1e9;
    int K = 0;
    int total = maxClusters;
    int totalDatos = datos.size();
    vector<Data *> result;
    vector<vector<Data *>> resultSet;
    if(totalDatos < maxClusters){
        total = totalDatos;
    }
    for(int k = 2; k <= total; k++){
        vector<Data *> centroides(k);
        vector<vector<Data *>> Sets(k);
        iniCentroides(datos, centroides);
        for(int data = 0; data < totalDatos; data++){
            float distanciaMenor = 1e9;
            int group = 0;
            for(int cluster = 0; cluster < k; cluster++){
                float distanciaNueva = distEuToCentroide(datos[data], centroides[cluster]);
                if(distanciaMenor > distanciaNueva){
                    distanciaMenor = distanciaNueva;
                    group = cluster;
                }
            }
            nuevoCentroide(centroides[group], datos[data]->dato);
            Sets[group].push_back(datos[data]);
            datos[data]->cluster = group;
        }

        float coeficiente = 0.0f;
        if(coeficiente != -1){
            for(int data = 0; data < totalDatos; data++){
                float datoA = a(Sets, datos[data]);
                float datoB = b(Sets, datos[data], k);
                coeficiente += (datoB-datoA)/max(datoB, datoA);
            }
        }
        coeficiente /= (float)totalDatos;

        for(int i = 0; i < Sets.size();i++){
            if(Sets[i].size() == 0){
                coeficiente = -1;
                break;
            }
        }
        float distRef = abs(1.0f-coeficiente);
        if(distRef < distTo1 && coeficiente != -1){
            distTo1 = distRef;
            K = k;
            result = centroides;
            resultSet = Sets;
        }
    }
    return make_pair(resultSet, make_pair(result, K));
}