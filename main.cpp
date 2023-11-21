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
    vector<Data> datos;
    vector<vector<float>> puntos;
    float Mx = -1e6;
    float My = -1e6;
    float mx = 1e6;
    float my = 1e6;
    int contador = 0;
    while (getline(archivo, linea) && contador <= 600)
    {
        stringstream strstr(linea);
        string number;
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
                nueva.longitud = n;
            }
            else if(c == 1){
                my = min(my,n);
                My = max(My,n);
                nueva.latitud = n;
            }
            c++;
        }
        nueva.dato = d;
        nueva.iterador = contador;
        datos.push_back(nueva);
        contador++;
    }
    float al = max(Mx-mx, My-my);
    int k = silueta(datos);
    cout << k << endl;
    //KmeanTree treek(k,0);
    //treek.Insert(puntos);
    /*Quadtree tree(Point(mx,my), al, 0);
    for(int i = 0; i < datos.size(); i++){
        cout << "Insertado punto " <<  i << " : " << datos[i].longitud << " " << datos[i].latitud << endl;
        tree.Insert(&datos[i]);
    }*/
    //tree.Union(treek);
    archivo.close();
    return 0;
}