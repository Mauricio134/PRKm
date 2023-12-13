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

KmeanTree::KmeanTree(){
    clusters.resize(divisiones);
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
    }

    for(int i = 0; i < setcomplete.size(); i++){
        for(int j = 0; j < setcomplete[i].size(); j++){
            setcomplete[i][j]->similitud = this;
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
        cout << "nuevo K" << endl;
        pair<vector<vector<Data *>>,pair<vector<Data *>, int>> k = silueta3(clusters[i]->Set);
        if(k.second.second == 0) continue;
        clusters[i]->next = new KmeanTree(k.second.second, altura+1);
        clusters[i]->next->Insert(clusters[i]->Set, k.second.first, k.first);
    }
}
/*void KmeanTree::clusterSelect(KmeanTree * &simil, vector<Data> conjunto){
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