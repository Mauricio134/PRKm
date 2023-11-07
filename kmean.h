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
    vector<Cluster *> clusters;
    int divisiones;
    KmeanTree();
    KmeanTree(int );
    void Insert(const vector<vector<float>> &);
};