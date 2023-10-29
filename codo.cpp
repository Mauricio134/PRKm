#include "librerias.h"
#include "kmean.h"

using namespace std;

// Función para calcular la distancia euclidiana entre dos puntos
float distancia(vector<float> a, vector<float> b) {
    float d = 0;
    for (int i = 0; i < a.size(); i++) {
        d += pow(a[i] - b[i], 2);
    }
    return sqrt(d);
}

// Función para calcular la suma de las distancias al cuadrado de cada punto al centroide de su cluster correspondiente
float sumaDistancias(vector<vector<float>> puntos, vector<int> clusters, vector<vector<float>> centroides) {
    float suma = 0;
    for (int i = 0; i < puntos.size(); i++) {
        suma += pow(distancia(puntos[i], centroides[clusters[i]]), 2);
    }
    return suma;
}


float Codo(vector<vector<float>> puntos){
    //freopen("output.txt", "w", stdout);
    vector<float> sumasDistancias(maxClusters);

    // Calcular la suma de las distancias al cuadrado para diferentes valores de k
    for (int k = 1; k <= maxClusters; k++) {
        // Inicializar los centroides aleatoriamente
        vector<vector<float>> centroides(k);
        for (int i = 0; i < k; i++) {
            centroides[i] = puntos[rand() % puntos.size()];
        }

        // Asignar los puntos a los clusters correspondientes
        vector<int> clusters(puntos.size());
        for (int i = 0; i < puntos.size(); i++) {
            float minDistancia = distancia(puntos[i], centroides[0]);
            int minCluster = 0;
            for (int j = 1; j < k; j++) {
                float d = distancia(puntos[i], centroides[j]);
                if (d < minDistancia) {
                    minDistancia = d;
                    minCluster = j;
                }
            }
            clusters[i] = minCluster;
        }

        // Actualizar los centroides
        for (int i = 0; i < k; i++) {
            vector<float> nuevoCentroide(puntos[0].size());
            int contador = 0;
            for (int j = 0; j < puntos.size(); j++) {
                if (clusters[j] == i) {
                    for (int l = 0; l < puntos[j].size(); l++) {
                        nuevoCentroide[l] += puntos[j][l];
                    }
                    contador++;
                }
            }
            for (int l = 0; l < nuevoCentroide.size(); l++) {
                nuevoCentroide[l] /= contador;
            }
            centroides[i] = nuevoCentroide;
        }

        sumasDistancias[k-1] = sumaDistancias(puntos, clusters, centroides);
    }

    for (int k = 1; k <= maxClusters; k++) {
        cout << k << " "<< sumasDistancias[k-1]/puntos.size() << endl;
    }

    int kOptimo = 1;
    float disminucionAnterior = sumasDistancias[0] - sumasDistancias[1];

    for (int k = 2; k <= maxClusters; k++) {
        float disminucionActual = sumasDistancias[k - 2] - sumasDistancias[k - 1];

        if (disminucionActual < disminucionAnterior * 0.1) {
            disminucionAnterior = disminucionActual;
            kOptimo = k - 1;
        }
    }
    return kOptimo;
}