#include "librerias.h"
#include "prquadtree.h"
#include "kmean.h"
#include "silueta2.cpp"
#include "prquadtree.cpp"
#include "kmean.cpp"

#define NOMBRE_ARCHIVO "meteorite.csv"

int main(){
    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ',';
    vector<Data *> datos;
    float Mx = -1e6;
    float My = -1e6;
    float mx = 1e6;
    float my = 1e6;
    int contador = 0;
    while (getline(archivo, linea) && contador <= 1000)
    {
        stringstream strstr(linea);
        string number;
        float longitud = 0;
        float latitud = 0;
        vector<float> d;
        int c = 0;
        Data nueva;
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
        contador++;
    }
    float al = max(Mx-mx, My-my);
    auto start = std::chrono::high_resolution_clock::now();
    int k = silueta2(datos);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Tiempo de ejecucion: " << duration.count() << " microsegundos." << std::endl;
    cout << k << endl;
    KmeanTree treek(k,0);
    treek.Insert(datos);
    /*Quadtree tree(Point(mx,my), al, 0);
    for(int i = 0; i < datos.size(); i++){
        cout << "Insertado punto " <<  i << " : " << datos[i].longitud << " " << datos[i].latitud << endl;
        tree.Insert(&datos[i]);
    }*/
    //tree.Union(treek);
    archivo.close();
    return 0;
}