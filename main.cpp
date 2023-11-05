#include "librerias.h"
#include "prquadtree.cpp"
#include "kmean.cpp"
#include "codo.cpp"
#include "silueta2.cpp"

#define NOMBRE_ARCHIVO "points.csv"

int main(){
    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ' ';
    vector<Data> datos;
    vector<vector<float>> puntos;
    float Mx = -1e6;
    float My = -1e6;
    float mx = 1e6;
    float my = 1e6;
    while (getline(archivo, linea))
    {
        stringstream strstr(linea);
        string number;
        vector<float> d;
        int c = 0;
        Data nueva;
        while(getline(strstr,number,' '))
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
        puntos.push_back(d);
        datos.push_back(nueva);
    }
    float al = max(Mx-mx, My-my);
    Quadtree tree(Point(mx,my), al);
    for(int i = 0; i < datos.size(); i++){
        tree.Insert(Data(datos[i].longitud, datos[i].latitud, datos[i].dato));
    }

    silueta2(puntos);
    archivo.close();
    return 0;
}