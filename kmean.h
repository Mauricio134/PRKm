#pragma once
#include "librerias.h"

class KmeanTree;
class Cluster;

class Cluster{
public:
    vector<Data *> Set;
    Data * valor;
    KmeanTree * next;
    Cluster();
};

class KmeanTree{
public:
    int divisiones;
    vector<Cluster *> clusters;
    int altura = 0;
    KmeanTree();
    KmeanTree(int , int);
    void Insert(vector<Data *> &, vector<Data *> &, vector<vector<Data *>> &);
    //void clusterSelect(KmeanTree * &, vector<Data> conjunto);
};