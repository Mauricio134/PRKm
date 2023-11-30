#include "librerias.h"

vector<vector<float>> prodMat(vector<Data * > &m1, vector<vector<float>> & m2, int indice){
    int m = m1.size();
    int k = m2.size();
    vector<vector<float>> res(m);

    for(int i = 0; i < m; i++){
        res[i] = vector<float>(k);
        for(int j = 0; j < indice; j++){
            res[i][j] = (accumulate(m1[i]->dato.begin(), m1[i]->dato.end(), 0.0f))*(accumulate(m2[j].begin(), m2[j].end(), 0.0f));
        }
    }
    return res;
}

vector<vector<float>> distMat(vector<Data * > &datos, vector<vector<float>> & centros, int indice){
    int datosFilas = datos.size();
    int centrosFilas = centros.size();

    auto producto = prodMat(datos, centros, indice);

    vector<vector<float>> matX(producto.size());
    vector<vector<float>> matY(producto.size());

    vector<vector<float>> temp(datos.size(), vector<float>(datos[0]->dato.size(), 0.0f));

    for (int i = 0; i < datos.size(); i++) {
        for (int j = 0; j < datos[0]->dato.size(); j++) {
            temp[i][j] = datos[i]->dato[j] * datos[i]->dato[j];
        }
    }

    for(int i = 0; i < datos.size(); i++){
        matX[i] = vector<float>(accumulate(temp[i].begin(), temp[i].end(), 0.0), producto[0].size());
    }

    temp = vector<vector<float>>(centros.size(), vector<float>(centros[0].size(), 0.0));
    vector<float> sumY(centros.size());
    for(int i = 0; i < indice; i++){
        sumY[i] = accumulate(temp[i].begin(), temp[i].end(), 0.0);
    }

    for(int i = 0; i < producto.size(); i++){
        matY[i] = sumY;
    }
    auto increment = [](float value) { return value*(-2); };
    for (auto& fila : producto) {
        transform(fila.begin(), fila.end(), fila.begin(), increment);
    }

    int rows = matX.size();
    int cols = matX[0].size();
    bool dimensionsMatch = rows == producto.size() && rows == matY.size() &&
                           cols == producto[0].size() && cols == matY[0].size();

    if (!dimensionsMatch) {
        cout << "Las dimensiones de las matrices no coinciden." << endl;
    }
    vector<vector<float>> result(rows, vector<float>(cols,0.0f));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matX[i][j] + producto[i][j] + matY[i][j];
        }
    }

    return result;

}

int probIndice(vector<float> & dist){
    long int sum = accumulate(dist.begin(), dist.end(), 0.0);
    long int randomSum = rand() % sum;

    for(int i = 0; i < dist.size(); i++){
        randomSum -= dist[i];
        if(randomSum < 0) return i;
    }
}

vector<vector<float>> iniCentros(vector<Data *> & datos, int k){
    vector<vector<float>> centros(k);
    centros[0] = datos[rand() % datos.size()]->dato;
    vector<float> minDistMat(datos.size());

    for(int i = 0; i < k-1; i++){
        auto dMat = distMat(datos, centros, i+1);
        for(int j = 0; j < datos.size(); j++){
            vector<float> temp(dMat[j].begin(), dMat[j].begin() + i + 1);
            minDistMat[j] = *(min_element(temp.begin(), temp.end()));
        }
        centros[i+1] = datos[probIndice(minDistMat)]->dato;
    }

    return centros;
}

vector<int> encontrarCentros(vector<Data*> & datos, vector<vector<float>> & centros, int k){
    vector<int> indice(datos.size());
    auto distancia = distMat(datos,centros,k);

    for(int i = 0; i < distancia.size(); i++){
        float minVal = distancia[i][0];
        indice[i] = 0;
        for(int j = 1; j < distancia[0].size(); j++){
            if(distancia[i][j] < minVal){
                minVal = distancia[i][j];
                indice[i] = j;
            }
        }
    }

    return indice;
}

vector<int> kmeansPP(vector<Data *> & datos, int k){
    int filas = datos.size();
    auto centros = iniCentros(datos, k);
    vector<int> viejosClusters(filas, -1);
    vector<vector<float>> c(k);
    int cont = 0;
    while(true){
        cont++;
        if(cont <= 30) return viejosClusters;
        auto nuevosClusters = encontrarCentros(datos, centros, k);
        if(equal(nuevosClusters.begin(), nuevosClusters.end(), viejosClusters.begin())) return nuevosClusters;
        viejosClusters = nuevosClusters;

        for (int i = 0; i < k; i++) {
            vector<vector<float>> temp(viejosClusters.size());
            for(int j = 0; j < viejosClusters.size(); j++){
                if(viejosClusters[j] == i){
                    temp[j] = datos[j]->dato;
                }
            }

            if(!temp.empty()){
                transform(temp[0].begin(), temp[0].end(), c[i].begin(), c[i].begin(), plus<float>());
                for(float & valor : c[i])
                    valor /= (float)temp.size();
            }
		}
        centros = c;
    }
}

float silueta(vector<Data *> &datos, vector<int> & clusters, int k){
    int filas = datos.size();
    vector<vector<float>> centros(k);
    int cont = 0;

    for(int i = 0; i < k; i++){
        vector<vector<float>> temp(clusters.size());
        for(int j = 0; j < clusters.size(); j++){
            if(clusters[j] == i){
                temp[j] = datos[j]->dato;
            }
        }

        if(!temp.empty()){
            transform(temp[0].begin(), temp[0].end(), centros[i].begin(), centros[i].begin(), plus<float>());
            for(float & valor : centros[i])
                valor /= (float)temp.size();
        }
    }

    auto distancia = distMat(datos, centros, k);

    vector<float> a(filas, 0.0f);
    vector<float> b(filas, 0.0f);

    vector<float> temp2(k - 1);

    float resultado = 0.0;

    for(int i = 0; i < filas; i++){
        a[i] = distancia[i][clusters[i]];

        int jindex = 0;
		for (int j = 0;j < k ;j++) if (clusters[i] != j) temp2[jindex++] = distancia[i][j];

        auto valor = min_element(temp2.begin(), temp2.end());

		b[i] = *valor;

        resultado += (b[i]-a[i])/max(a[i], b[i]);
    }

    resultado /= (float) filas;
    return resultado;
}