#pragma once
#include "librerias.h"

class KmeanTree;
class Cluster;

class Cluster{
public:
    vector<vector<float>> Set;
    vector<float> valor;
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
    void Insert(const vector<vector<float>> &);
    void clusterSelect(KmeanTree *, vector<Data> conjunto);
};