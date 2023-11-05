#include "librerias.h"
#include "kmean.h"
#include "silueta2.cpp"

int amount = 0;

float distanciaEu(vector<float> centro, vector<float> dato){
    int suma = 0;
    for(int i = 0; i < dato.size(); i++){
        suma += pow((centro[i] - dato[i]),2);
    }
    return (float) sqrt(suma);
}

vector<float> nuevoCen(vector<vector<float>> conjunto){
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

Cluster::Cluster(){
    next = new KmeanTree();
}

KmeanTree::KmeanTree(){
    clusters.resize(amount);
}

KmeanTree::KmeanTree(int k){
    clusters.resize(k);
    amount = k;
    for(int i = 0; i < amount; i++) clusters[i] = new Cluster();
}

void KmeanTree::Insert(const vector<vector<float>> & datos){
    //seleccionar Centroides
    for(int i = 0; i < amount; i++){
        clusters[i]->valor = datos[rand() % datos.size()];
    }

    for(int i = 0; i < datos.size(); i++){
        float distanciaMin = 1e9;
        float indice = 0;
        for(int j = 0; j < clusters.size(); j++){
            float nuevaDist = distanciaEu(datos[i], clusters[j]->valor);
            if(nuevaDist < distanciaMin){
                distanciaMin = nuevaDist;
                indice = j;
            }
        }
        clusters[indice]->Set.push_back(datos[i]);
        clusters[indice]->valor = nuevoCen(clusters[indice]->Set);
    }

    for(int i = 0; i < amount; i++){
        clusters[i]->next = new KmeanTree(amount);
        if(clusters[i]->Set.size() == 1 || clusters[i]->Set.size() == 0){
            return;
        }
        clusters[i]->next->Insert(clusters[i]->Set);
    }
}