#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

//srand() para usar la semilla
using namespace std;

int m = 40;
float pmutacion_threshold = 0.2;
float pr = 0.1;
int seed = 20;


void main(int argc, char* argv[]) {
	int quorum = 74;
	if (argc > 1) {
		m = stoi(argv[0]);
		pmutacion_threshold = stod(argv[1]);
		pr = stod(argv[2]);
		seed = stoi(argv[3]);
	}
	int** matrix = (int**)malloc(2*sizeof(int*));
	int* linea = new int[2];
	matrix[0] = linea;
	linea[0] = 0;
	linea[1] = 1;

	cout << matrix[0][0];
	cout << matrix[0][1];

	int* linea = new int[2];
	matrix[1] = linea;
	linea[0] = 3;
	cout << matrix[1][0];
}