#include <iostream>
#include <json/value.h>
#include <fstream>

using namespace std;

int main(){

	ifstream data("ejemplo2.json", ifstream::binary);

    int m=40;

    int cromosoma[m][154]={};
    float fitnessPob[m]={};

    int cromosoma_nuevo[m][154]={};
    float fitnessPob_nuevo[m]={};

    float fitnessEvol[]={};

    int itera[]={};

    int crossover12[]={};
    int crossover21[]={};
    //prueba funcionamiento arreglos
    cromosoma[0][0]=1;
    fitnessPob[0]=0.1;
    cromosoma_nuevo[0][0]=2;
    //memoria XD (Buscar como trabajar bien con la memoria)
    cout << data;
    return 0;
}
