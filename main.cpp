#include "librerias.h"
#include "prquadtree.h"
#include "kmean.h"
#include "silueta3.cpp"
#include "prquadtree.cpp"
#include "kmean.cpp"

#define NOMBRE_ARCHIVO "test_final.csv"

int main(){
    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ';';
    vector<Data *> datos;
    float Mx = -1e6;
    float My = -1e6;
    float mx = 1e6;
    float my = 1e6;
    int contador = 0;
    while (getline(archivo, linea))
    {
        stringstream strstr(linea);
        string number;
        float longitud = 0;
        float latitud = 0;
        int c = 0;
        vector<float> d;
        while(getline(strstr,number,delimitador))
        {
            float n;
            istringstream(number) >> n;
            if(c > 1){
                d.push_back(n);
            }
            else if(c == 0){
                mx = min(mx,n);
                Mx = max(Mx,n);
                longitud = n;
            }
            else if(c == 1){
                my = min(my,n);
                My = max(My,n);
                latitud = n;
            }
            c++;
        }
        datos.push_back(new Data(longitud, latitud, d));
        datos.front()->etiqueta = contador;
        contador++;
    }
    float al = max(Mx-mx, My-my);
    pair<vector<vector<Data *>>,pair<vector<Data *>, int>> k = silueta3(datos);
    KmeanTree treek(k.second.second,0);
    treek.Insert(datos, k.second.first, k.first);
    Quadtree tree(Point(mx,my), al, 0);
    for(int i = 0; i < datos.size(); i++){
        tree.Insert(datos[i]);
    }
    set<Data *> grupo;
    float menorX = 18.0f;
    float mayorX = 197.0f;
    float menorY = 20.0f;
    float mayorY = 45.0f;
    tree.rangeQuery(menorX, menorY, mayorX, mayorY, grupo);

    vector<Cluster *> parecidos = tree.Similarity(menorX, menorY, mayorX, mayorY, grupo);
    archivo.close();
    return 0;
}