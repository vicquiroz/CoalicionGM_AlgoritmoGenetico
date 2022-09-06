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
//inicializador de generador de random
random_device rng;
default_random_engine generator(seed);
mt19937 mt{ rng() };

//inicializador de random, tanto para int's como para float's
uniform_int_distribution<int> uni;
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

// spanning tree
void create_crom(int* cromosoma, float** matDis, int n, int quorum, int init_index)
{
	// Array que almacena los nodos
	// 
	//int dparent[n];
	int* dparent = new int[n];
	//  Almacena la distancia entre el punto (a,b) de los dparent seleccionados
	//float d[n]; 
	float* d = new float[n];
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

	sort_bubble(cromosoma, quorum);
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

//funcion para suamar los valores de un arreglo
float suma(float* array, int largo)
{
	float suma = 0;
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
int smallest_greater(float* seq, int largo, float value)
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
void order(float* fitness, int** cromosoma, int quorum, int m)
{
	int temp = 0;
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

//funcion que retorna el mayor valor de un arreglo
int maximo(int* array, int largo)
{
	int maxi = 0;
	for (size_t i = 0; i < largo - 1; i++)
	{
		if (array[i] > array[i + 1])
			maxi = array[i];
		else
			maxi = array[i + 1];
	}
	return maxi;
}

//funcion que retorna la posicion de los valores que sean los mayores de un arreglo
int which_max(float* array, int largo)
{
	int maxi = array[0];
	for (size_t i = 0; i < largo - 1; i++)
	{
		if (array[i] > array[i + 1])
		{
			maxi = array[i];
		}
		else
		{
			maxi = array[i + 1];
		}
	}
	for (int i = 0; i < largo; i++)
	{
		if (array[i] == maxi)
			return i;
	}
	return -1;
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

int main(int argc, char* argv[])
{
	//llamada de JSON

	//para camara de diputados chile
	//ifstream archivo("ejemplo2.json");
	//json data = json::parse(archivo);

	//para parlamento de estados unidos
	ifstream archivo("ingles.json");
	json data = json::parse(archivo);

	//Poblacion Inicial
	ofstream poblacionInit;
	//JSON
	//poblacionInit.open("poblacionInit.json");
	poblacionInit.open("RH0750158.csv");
	//se crea y abre el archivo de salida

	//string pobInit = "{";
	string pobInit = "";
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

	//inicializador de Poblacion y fitness
	int** cromosoma = (int**)malloc(m * sizeof(int*));
	float* fitnessPob = nullptr;
	fitnessPob = (float*)malloc(m * sizeof(float));

	//inicializacion de cromosomas
	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int*)malloc(quorum * sizeof(int));
	}

	///////////////////////////////////////////
	// spanning tree Inicio de población
	///////////////////////////////////////////
	/*
	pop_select = uniform_int_distribution<int>(0,n-1);
	for (int i = 0; i < m; i++){
		create_crom(cromosoma[i],matDis,n,quorum,pop_select(mt));
	}
	for (size_t i = 0; i < m; i++)
	{
		//sample(cromosoma[i], n, quorum);
		//sort_bubble(cromosoma[i], quorum);
		//cout << eval_sol(cromosoma[i], matDis, quorum) << endl;
		fitnessPob[i] = eval_sol(cromosoma[i], matDis, quorum);
	}

	//ordenamiento de fitness y cromosomas
	order(fitnessPob, cromosoma, quorum, m);
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
	for (int i = 0; i < m; i++) {
		memcpy(cromosoma[i], cromosoma_n[fitnessPobInitIndex[i]], sizeof(int) * quorum);
		fitnessPob[i] = fitnessPobInit[fitnessPobInitIndex[i]];
	}
	//CREAR JSON
	/*for (int i = 0; i < n; i++) {
		pobInit = pobInit +'"'+ to_string(i) +'"' + ":[";
		for (int j = 0; j < quorum; j++) {
			pobInit = pobInit + to_string(cromosoma_n[fitnessPobInitIndex[i]][j]) + ",";
		}
		replace(pobInit.end() - 1, pobInit.end(), ',', ']');
		pobInit = pobInit + "," + "\n";
	}
	replace(pobInit.end() - 1, pobInit.end(), ',', ' ');
	poblacionInit << pobInit << "}";
	*/
	//GENERAR CSV
	pobInit = pobInit + "fitness,id_dip,nombre_dip,X,Y,";
	for (int j = 0; j < quorum; j++) {
		pobInit = pobInit +"V"+ to_string(j) + ",";
	}
	replace(pobInit.end() - 1, pobInit.end(), ',', '\n');
	string temp;
	string empty = "";
	for (int i = 0; i < n; i++) {
		pobInit = pobInit + to_string(fitnessPobInit[fitnessPobInitIndex[i]]) + ",";
		pobInit = pobInit + to_string(fitnessPobInitIndex[i]) +",";
		temp = to_string(data["rollcalls"][0]["votes"][fitnessPobInitIndex[i]]["name"]);
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
		pobInit=pobInit+temp + ",";
		pobInit = pobInit + to_string(data["rollcalls"][0]["votes"][fitnessPobInitIndex[i]]["x"])+"," + to_string(data["rollcalls"][0]["votes"][fitnessPobInitIndex[i]]["y"]) + ",";
		for (int j = 0; j < quorum; j++) {
			pobInit = pobInit + to_string(cromosoma_n[fitnessPobInitIndex[i]][j]) + ",";
		}
		replace(pobInit.end() - 1, pobInit.end(), ',', '\n');
	}
	poblacionInit << pobInit;
	return EXIT_SUCCESS;
}