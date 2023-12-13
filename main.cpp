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
    while (getline(archivo, linea) && contador <= 4000)
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
    auto start = std::chrono::high_resolution_clock::now();
    pair<vector<vector<Data *>>,pair<vector<Data *>, int>> k = silueta3(datos);
    cout << k.second.second << endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Tiempo de ejecucion: " << duration.count() << " microsegundos." << std::endl;
    KmeanTree treek(k.second.second,0);
    treek.Insert(datos, k.second.first, k.first);
    Quadtree tree(Point(mx,my), al, 0);
    for(int i = 0; i < datos.size(); i++){
        cout << "Insertado punto " <<  i << " : " << datos[i]->longitud << " " << datos[i]->latitud << endl;
        tree.Insert(datos[i]);
    }
    set<Data *> grupo;
    Point inicio(-18,20);
    int anchura = 50;
    tree.rangeQuery(inicio, anchura, grupo);

    set<Data *> parecidos = tree.Similarity(inicio, anchura, grupo);
    //tree.Union(treek);
    archivo.close();
    return 0;
}