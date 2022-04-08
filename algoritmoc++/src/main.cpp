#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

//srand() para usar la semilla
using namespace std;

int m = 5;
float pmutacion_threshold = 0.2;
float pr = 0.1;
int seed = 20;

void cambio(int** matriz, int pos, int largo)
{
    // int *new_point = new int[n];
    for (int i = 0; i < largo; i++)
    {
        matriz[pos][i] = 0;
    }
}


void main(int argc, char* argv[]) {
	int quorum = 10;
	if (argc > 1) {
		m = stoi(argv[0]);
		pmutacion_threshold = stod(argv[1]);
		pr = stod(argv[2]);
		seed = stoi(argv[3]);
	}

    int** cromosoma=(int**)malloc(m * sizeof(int*));
    //hay que usar m no un valor fijo
    //int* cromosoma[2];
    //cromosoma[0] = array;
   
    for (size_t i = 0; i < m; i++)
    {
        cromosoma[i] = (int*)malloc(quorum * sizeof(int));

    }

    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < quorum; j++)
        {
            cout << cromosoma[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cambio(cromosoma, 4, quorum);

    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < quorum; j++)
        {
            cout << cromosoma[i][j] << " ";
        }
        cout << endl;
    }
    

}