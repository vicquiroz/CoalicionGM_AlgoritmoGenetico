//llamada de librerias
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <random>
#include <math.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <cmath>
#include <cfloat>
//acortadores
using namespace std;
using json = nlohmann::json;

//parametros iniciales
int m = 54;
float pmutacion_threshold = 0.36;
float pr = 0.1652927;
//int seed = time(NULL);
int seed = 1234;
auto tInicial = chrono::high_resolution_clock::now();
//inicializador de generador de random
random_device rng;
default_random_engine generator(seed);
mt19937 mt{ rng() };

//inicializador de random, tanto para int's como para float's
uniform_real_distribution<double> uni2;


//funcion para ordenar un arreglo de menor a mayor
void sort_bubble(int* array, int largo)
{
	int temp = 0;
	for (size_t i = 0; i < largo; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < largo - i - 1; j++)
		{
			if (array[j] > array[j + 1])
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
				already_sorted = false;
			}
		}
		if (already_sorted)
			break;
	}
}

void sort_BubbleIndex(int* arrayIndex, float* arrayDist, int n, int quorum) {
	int temp = 0;
	//int orderIndex[n];
	int* orderIndex = new int[n];
	for (size_t i = 0; i < n; i++)
		orderIndex[i] = i;

	for (size_t i = 0; i < n; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < n - i - 1; j++)
		{
			if (arrayDist[orderIndex[j]] > arrayDist[orderIndex[j + 1]])
			{
				temp = orderIndex[j];
				orderIndex[j] = orderIndex[j + 1];
				orderIndex[j + 1] = temp;
				already_sorted = false;
			}
		}
		if (already_sorted)
			break;
	}
	memcpy(arrayIndex, orderIndex, sizeof(int) * quorum);
}


//***********funciones***********//
// funcion spanning tree
uniform_int_distribution<int> pop_select;

// Encuentra la minima distancia entre los vertices que no han sido guardados
// dentro del minimum spanning tree
float minDist(float d[], bool genSet[], int n)
{
	float min = FLT_MAX, min_index;
	for (int v = 0; v < n; v++)
		if (genSet[v] == false && d[v] < min)
			min = d[v], min_index = v;
	return min_index;
}


// Selecciona los n/2+1 nodos mas cercano al nodo inicial
void minDistEdge(int* arrayIndex, float* arrayDist, int n, int quorum)
{
	//bool genSet[n]; 
	bool* genSet = new bool[n];
	for (size_t i = 0; i < n; i++) {
		genSet[i] = false;
	}
	for (size_t i = 0; i < quorum; i++) {
		int u = minDist(arrayDist, genSet, n);
		genSet[u] = true;
		arrayIndex[i] = u;
	}
}



//funcion para calcular la distancia entre dos puntos
float dis_euc(float x1, float y1, float x2, float y2)
{
	float calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (float)2);
	return calculo;
}

//evaluar los cromosomas y calcular su fitness
float eval_sol(int* pos, float** mat, int largo) {
	float suma = 0;
	for (size_t i = 0; i <= (largo - 2); i++)
	{

		for (size_t j = i + 1; j <= (largo - 1); j++)
		{
			suma = suma + mat[pos[i]][pos[j]];
		}
	}
	return suma;
}

int main(int argc, char* argv[])
{
	
	//llamada de JSON

	//para camara de diputados chile
	//ifstream archivo("ejemplo2.json");
	//json data = json::parse(archivo);

	//para parlamento de estados unidos
	ifstream archivo("ingles.json");
	json data = json::parse(archivo);

	
	//se crea y abre el archivo de salida
	//Poblacion Inicial
	ofstream poblacionInit;
	poblacionInit.open("poblacionInit.txt");
	string pobInit = "";

	ofstream matrizDistancia;
	matrizDistancia.open("matrizDistancia.txt");
	string matDist = "";

	//de los JSON se obtiene la cantidad de diputados
	//para parlamento de estados unidos
	int n = data["rollcalls"][0]["votes"].size();

	//para camara de diputados chile
	//int n = data["diputados"].size();

	//creacion de la matriz de distancia
	float** matDis = (float**)malloc(n * sizeof(float*));
	for (size_t i = 0; i < n; i++)
	{
		matDis[i] = (float*)malloc(n * sizeof(float));
	}

	//rellenado de la matriz de distancia
	// Matriz rellenando por completo
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			matDis[i][j] = dis_euc(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]);
		}
	}
	//para parlamento de estados unidos
	/*
	for (size_t i = 0; i <= (n - 2); i++)
	{

		//cout << i << endl;
		for (size_t j = i + 1; j <= (n - 1); j++)
		{
			matDis[i][j] = dis_euc(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]);
		}
	}
	*/
	//para camara de diputados chile
	/*for (size_t i = 0; i <= (n - 2); i++)
	{

		//cout << i << endl;
		for (size_t j = i + 1; j <= (n - 1); j++)
		{
			matDis[i][j] = dis_euc(data["diputados"][i]["coordX"], data["diputados"][i]["coordY"], data["diputados"][j]["coordX"], data["diputados"][j]["coordY"]);
		}
	}*/


	//inicializacion de quorum
	//para parlamento de estados unidos
	int quorum = trunc(n / 2) + 1;
	//para camara de diputados chile
	//int quorum = 74;

	//revision de parametros de entrada
	if (argc > 1)
	{
		m = stoi(argv[1]);
		pmutacion_threshold = stod(argv[2]);
		pr = stod(argv[3]);
		seed = stoi(argv[4]);
	}
	//inicializacion de semilla
	mt.seed(seed);

	//generador de float's random entre 0 y 1
	uni2 = uniform_real_distribution<double>(0, 1);
	/*
	//inicializador de Poblacion y fitness
	int** cromosoma = (int**)malloc(m * sizeof(int*));
	float* fitnessPob = nullptr;
	fitnessPob = (float*)malloc(m * sizeof(float));

	//inicializacion de cromosomas
	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int*)malloc(quorum * sizeof(int));
	}
	*/


	/////////////////////////////////////
	////// Ordenamiento algoritmo B
	/////////////////////////////////////
	int** cromosoma_n = (int**)malloc(n * sizeof(int*));
	for (size_t i = 0; i < n; i++)
	{
		cromosoma_n[i] = (int*)malloc(quorum * sizeof(int));
	}

	float* fitnessPobInit = (float*)malloc(n * sizeof(float));
	int* fitnessPobInitIndex = (int*)malloc(n * sizeof(int));

	for (int j = 0; j < n; j++) {
		minDistEdge(cromosoma_n[j], matDis[j], n, quorum);
		sort_bubble(cromosoma_n[j], quorum);
		fitnessPobInit[j] = eval_sol(cromosoma_n[j], matDis, quorum);
	}

	/// Ordena los resultados
	sort_BubbleIndex(fitnessPobInitIndex, fitnessPobInit, n, n);
	// Traspasa los m mejores de toda la población inicial
	/*for (int i = 0; i < m; i++) {
		memcpy(cromosoma[i], cromosoma_n[fitnessPobInitIndex[i]], sizeof(int) * quorum);
		fitnessPob[i] = fitnessPobInit[fitnessPobInitIndex[i]];
	}*/
	auto tFinal = chrono::high_resolution_clock::now();
	double tTomado = chrono::duration_cast<chrono::nanoseconds>(tFinal - tInicial).count();
	tTomado *= 1e-9;
	cout << fixed << tTomado << setprecision(9)<<"\n";
	tInicial = chrono::high_resolution_clock::now();
	//GENERAR CSV
	string temp;
	string empty = "";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < quorum; j++) {
			pobInit = pobInit + to_string(cromosoma_n[fitnessPobInitIndex[i]][j]) + ",";
		}
		pobInit = pobInit + to_string(fitnessPobInit[fitnessPobInitIndex[i]])+"\n";
	}
	poblacionInit << pobInit;
	poblacionInit.close();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			matDist = matDist + to_string(matDis[i][j]) + ",";
		}
		replace(matDist.end() - 1, matDist.end(), ',', '\n');
	}
	matrizDistancia << matDist;
	matrizDistancia.close();
	tFinal = chrono::high_resolution_clock::now();
	tTomado = chrono::duration_cast<chrono::nanoseconds>(tFinal - tInicial).count();
	tTomado *= 1e-9;
	cout << fixed << tTomado << setprecision(9)<<"\n";

	/*********************************************************************************/
	ifstream matDistAr("matrizDistancia.txt");
	ifstream pobInitAr("poblacionInit.txt");

	float** matDisX = (float**)malloc(n * sizeof(float*));
	for (size_t i = 0; i < n; i++)
	{
		matDisX[i] = (float*)malloc(n * sizeof(float));
	}

	int** cromosoma_x = (int**)malloc(n * sizeof(int*));
	for (size_t i = 0; i < n; i++)
	{
		cromosoma_x[i] = (int*)malloc(quorum * sizeof(int));
	}

	string matDistLine;
	int cx = 0;
	while (getline(matDistAr, matDistLine)) {
		stringstream linestream(matDistLine);
		string tempData;
		for (size_t i = 0; i < n; i++)
		{
			getline(linestream, tempData, ",");
			matDisX[cx][i] = stof(tempData);
		}
		cx++;
	}
	info_school.close();
	return EXIT_SUCCESS;
}