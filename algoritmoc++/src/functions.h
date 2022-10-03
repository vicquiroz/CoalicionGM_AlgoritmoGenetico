
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
//acortadores
using namespace std;
int seed = 2417704;
// Inicialización de randoms
uniform_int_distribution<int> uni;
uniform_real_distribution<double> uni2;
uniform_int_distribution<int> pop_select;
random_device rng;
default_random_engine generator(seed);
mt19937 mt{ rng() };
struct SolutionStructure {
	int* coalition;
	double fitness;
};

bool array_sort(int const& lvd, int const& rvd)
{
	return lvd < rvd;
}
bool vector_initial_solutions_sort(SolutionStructure const& lvd, SolutionStructure const& rvd)
{
	return lvd.fitness < rvd.fitness;
}

//***********funciones***********//
// Encuentra la minima distancia entre los vertices que no han sido guardados
// dentro del minimum spanning tree
int minDist(double d[], bool genSet[], int n)
{
	double min = FLT_MAX;
	int min_index;
	for (int v = 0; v < n; v++)
		if (genSet[v] == false && d[v] < min)
			min = d[v], min_index = v;
	return min_index;
}

// spanning tree
void create_crom(int* cromosoma, double** matDis, int n, int quorum, int init_index)
{
	// Array que almacena los nodos
	// 
	//int dparent[n];
	int* dparent = new int[n];
	//  Almacena la distancia entre el punto (a,b) de los dparent seleccionados
	//double d[n]; 
	double* d = new double[n];
	// Vertices que son partes del mininmum spanning tree
	//bool genSet[n]; // vertices que se incluyen
	bool* genSet = new bool[n];
	// Inicializa todas las distancias con un numero muy alto
	for (int i = 0; i < n; i++)
		d[i] = FLT_MAX, genSet[i] = false;
	// Aqui se define con que vertice inciar (El indice de d), donde tendra peso 0 
	// Como es el primer indice seleccionado en dparent se le asigna cualquier valor 
	// ya que no esta conectado con ningun vertice.
	d[init_index] = 0;
	dparent[init_index] = -1;

	// El numero de vertices que se seleccionan para el minimum spanning tree es de tamaño quorum
	for (int count = 0; count < quorum; count++)
	{
		// Encuentra la minima distancia, como inician con una distancia alta
		// selecciona el primer nodo
		// para el resto de las iteraciones ya tienen 
		int u = minDist(d, genSet, n);
		// Habilita la bandera de que el vertice esta dentro del minimum spanning tree
		genSet[u] = true;
		// Almacena dentro del cromosoma el vertice
		cromosoma[count] = u;
		// Actualizo los bordes adjacente al vertice u
		for (int v = 0; v < n; v++) {
			// graph[u][v] is non zero only for adjacent vertices of m 
			// genSet[v] is false for vertices not yet included in MST 
			// Update the d only if graph[u][v] is smaller than d[v] 
			if (matDis[u][v] && genSet[v] == false && matDis[u][v] < d[v])
				dparent[v] = u, d[v] = matDis[u][v];
		}
	}

	//sort_bubble(cromosoma, quorum);
	sort(cromosoma, cromosoma+quorum, &array_sort);
}


// Selecciona los n/2+1 nodos mas cercano al nodo inicial
void minDistEdge(int* arrayIndex, double* arrayDist, int n, int quorum)
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
double dis_euc(double x1, double y1, double x2, double y2)
{
	double calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (double)2);
	return calculo;
}

//evaluar los cromosomas y calcular su fitness
double eval_sol(int* pos, double** mat, int largo) {
	double suma = 0;
	for (size_t i = 0; i <= (largo - 2); i++)
	{

		for (size_t j = i + 1; j <= (largo - 1); j++)
		{
			suma = suma + mat[pos[i]][pos[j]];
		}
	}
	return suma;
}

//funcion para suamar los valores de un arreglo
double suma(double* array, int largo)
{
	double suma = 0;
	for (size_t i = 0; i <= largo; i++)
	{
		suma = suma + array[i];
	}
	return suma;
}

//funcion para obtener el numero de booleanos "TRUE" en un arreglo
int suma_bool(bool* array, int largo)
{
	int suma = 0;
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i])
		{
			suma++;
		}
	}
	return suma;
}

//funcion para obtener la posicion de el primer valor en un arreglo que sea más pequeño que un valor dado
int smallest_greater(double* seq, int largo, double value)
{
	int i = 0;
	bool flag = true;
	while (flag)
	{
		i++;
		if (seq[i] > value)
		{
			return i;
		}
		else
		{
			if (i == largo)
			{
				break;
			}
		}
	}
	return(int)EXIT_FAILURE;
}

//funcion para llenar un arreglo con numeros aleatorios entre un rango dado
void sample(int* arreglo, int limite, int largo)
{
	int i = 0;
	bool repetido = false;
	int valor = 0;
	if (limite == 0) {
		uni = uniform_int_distribution<int>(0, limite);
	}
	else {
		uni = uniform_int_distribution<int>(0, limite - 1);
	}
	while (i < largo)
	{
		valor = uni(mt);
		for (size_t j = 0; j < i; j++)
		{
			if (valor == arreglo[j])
				repetido = true;
		}
		if (repetido == false)
		{
			arreglo[i] = valor;
			i++;
		}
		repetido = false;
	}
}


//funcion para ordenar un arreglo de menor a mayor y aplica esos cambios a una matriz
void order(double* fitness, int** cromosoma, int quorum, int m)
{
	double temp = 0;
	int* arrTemp = (int*)malloc(quorum * sizeof(int));
	for (size_t i = 0; i < m; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < m - i - 1; j++)
		{
			if (fitness[j] > fitness[j + 1])
			{
				temp = fitness[j];
				fitness[j] = fitness[j + 1];
				fitness[j + 1] = temp;

				memcpy(arrTemp, cromosoma[j], sizeof(int) * quorum);
				memcpy(cromosoma[j], cromosoma[j + 1], sizeof(int) * quorum);
				memcpy(cromosoma[j + 1], arrTemp, sizeof(int) * quorum);
				already_sorted = false;
			}
		}
		if (already_sorted)
			break;

	}
	free(arrTemp);
}

//funcion para retornar los valores que estan en el arreglo1 pero no en el arreglo2 (valores)
int** notin(int* array1, int* array2, int largo1, int largo2)
{
	int* temp = (int*)malloc(sizeof(int) * largo1);
	int cont = 0;
	bool flag = false;
	for (int i = 0; i < largo1; i++)
	{
		for (int j = 0; j < largo2; j++)
		{
			if (array1[i] == array2[j])
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			temp[cont] = array1[i];
			cont++;
		}
		flag = false;
	}
	int** ret = (int**)malloc(sizeof(int*) * 2);
	ret[0] = (int*)malloc(sizeof(int) * cont);
	ret[1] = (int*)malloc(sizeof(int) * 1);
	ret[1][0] = cont;
	for (size_t i = 0; i < cont; i++)
	{
		ret[0][i] = temp[i];
	}

	free(temp);
	return ret;
}

//funcion para retornar un arreglo de booleanos, donde TRUE significa que estan en ambos arreglos y FALSE significa que esta en el arreglo1 pero no en el arreglo2
bool* in_boolean(int* array1, int* array2, int largo1, int largo2)
{
	bool* result = (bool*)malloc(sizeof(bool) * largo1);
	int result_len = 0;
	bool flag = false;
	for (size_t i = 0; i < largo1; i++)
	{
		for (size_t j = 0; j < largo2; j++)
		{
			if (array1[i] == array2[j])
			{
				result[result_len] = true;
				result_len++;
				flag = true;
			}
		}
		if (flag == false)
		{
			result[result_len] = false;
			result_len++;
		}
		flag = false;
	}
	return result;
}

//funcion que retorna el menor valor de un arreglo
int minimo(int* array, int largo)
{
	int mini = 0;
	for (size_t i = 0; i < largo - 1; i++)
	{
		if (array[i] < array[i + 1])
			mini = array[i];
		else
			mini = array[i + 1];
	}
	return mini;
}


//funcion que dado un arreglo de booleanos retorna la posicion de los valores que sean TRUE
int** which(bool* array, int largo)
{
	int cont = 0;
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i])
			cont++;
	}
	int** ret = nullptr;
	ret = (int**)malloc(sizeof(int*) * 2);
	ret[0] = (int*)malloc(sizeof(int) * cont);
	ret[1] = (int*)malloc(sizeof(int));
	int cont2 = 0;
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i])
		{
			ret[0][cont2] = i;
			cont2++;
		}
	}
	ret[1][0] = cont2;
	return ret;
}

//funcion que crea y retorna un arreglo de enteros
int* crear_arreglo(int largo)
{
	int* arreglo = (int*)malloc(sizeof(int) * largo);
	for (size_t i = 0; i < largo; i++)
	{
		arreglo[i] = i;
	}

	return arreglo;
}

//funcion que dado un arreglo este es rellenado con valores aleatorios
void sample_arreglo(int* arreglo, int cant, int* valores, int largo)
{
	int indice = 0;
	int valor = 0;
	int i = 0;
	bool repetido = false;
	uni = uniform_int_distribution<int>(0, largo - 1);
	while (i < cant)
	{
		indice = uni(mt);
		valor = valores[indice];
		for (size_t j = 0; j < i; j++)
		{
			if (valor == arreglo[j])
				repetido = true;
		}
		if (repetido == false)
		{
			arreglo[i] = valor;
			i++;
		}
		repetido = false;
	}
}
