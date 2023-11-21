/*#include "librerias.h"
#include "kmean.h"
#include "silueta2.cpp"


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
    clusters.resize(divisiones);
}

KmeanTree::KmeanTree(int k, int nuevah){
    clusters.resize(k);
    divisiones = k;
    altura = nuevah;
    for(int i = 0; i < k; i++) clusters[i] = new Cluster();
}

void KmeanTree::Insert(const vector<vector<float>> & datos){
    //seleccionar Centroides
    for(int i = 0; i < divisiones; i++){
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

    for(int i = 0; i < divisiones; i++){
        if(altura > mxAltura){
            return;
        }
        int nuevoK = silueta(clusters[i]->Set);
        clusters[i]->next = new KmeanTree(nuevoK, altura+1);
        clusters[i]->next->Insert(clusters[i]->Set);
    }
}

void KmeanTree::clusterSelect(KmeanTree * &simil, vector<Data> conjunto){
    if(altura < mxAltura){
        vector<pair<float, int>> porcentajes(clusters.size());
        if(divisiones < 2){
            float aumentar = (float)1/(float)conjunto.size();
            vector<float> diferencia(clusters.size(), (float)-1/(float)clusters.size());
            for(int i = 0; i < conjunto.size(); i++){
                float distActual = 1e9;
                int indice = 0;
                for(int j = 0; j < clusters.size(); j++){
                    float distNueva = distanciaEu(conjunto[i].dato, clusters[j]->valor);
                    if(distNueva < distActual){
                        distActual = distNueva;
                        indice = j;
                    }
                }
                porcentajes[indice].first += aumentar;
                porcentajes[indice].second = indice;
                diferencia[indice] += aumentar;
            }
            float sumatoria = 0;
            for(int i = 0; i < diferencia.size(); i++){
                diferencia[i] *= diferencia[i];
                sumatoria += diferencia[i];
            }
            float response = ( ( sqrt( (float)sumatoria/(float)clusters.size() ) )/( (float)1/(float)clusters.size() ) )*(float)100;
            sort(porcentajes.begin(), porcentajes.end(), [](pair<float, int> p1, pair<float, int> p2){return p1.first < p2.first;});
            if(response < 56){
                simil = this;
                return;
            }
        }
        clusters[porcentajes.begin()->second]->next->clusterSelect(simil,conjunto);
        return;
    }
    simil = this;
    return;
}*/