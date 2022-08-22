#include <coalicionGM.hpp>
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
#include <bits/stdc++.h>
//acortadores
using namespace std;
using json = nlohmann::json;

//parametros iniciales
int m = 50;
float pmutacion_threshold = 0.4;
float pr = 0.14;
int seed = time(NULL);

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

void sort_BubbleIndex(int* arrayIndex, float *arrayDist, int n, int quorum){
    int temp = 0;
    int orderIndex[n];
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
    memcpy(arrayIndex, orderIndex,sizeof(int)*quorum);
}


//***********funciones***********//
// funcion spanning tree
uniform_int_distribution<int> pop_select;

// Encuentra la minima distancia entre los vertices que no han sido guardados
// dentro del minimum spanning tree
float minDist(float d[], bool genSet[],int n) 
{ 
    float min = FLT_MAX, min_index; 
    for (int v = 0; v < n; v++) 
        if (genSet[v] == false && d[v] < min) 
            min = d[v], min_index = v; 
    return min_index; 
} 

// spanning tree
void create_crom(int *cromosoma,float **matDis,int n,int quorum,int init_index) 
{ 
    // Array que almacena los nodos
    // 
    int dparent[n];
    //  Almacena la distancia entre el punto (a,b) de los dparent seleccionados
    float d[n]; 
    // Vertices que son partes del mininmum spanning tree
    bool genSet[n]; // vertices que se incluyen
    // Inicializa todas las distancias con un numero muy alto
    for (int i = 0; i < n; i++) 
        d[i] = FLT_MAX, genSet[i] = false; 
    // Aqui se define con que vertice inciar (El indice de d), donde tendra peso 0 
    // Como es el primer indice seleccionado en dparent se le asigna cualquier valor 
    // ya que no esta conectado con ningun vertice.
    d[init_index] =0;
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
  
    sort_bubble(cromosoma,quorum);
} 


// Selecciona los n/2+1 nodos mas cercano al nodo inicial
void minDistEdge(int* arrayIndex, float *arrayDist, int n, int quorum) 
{ 
    bool genSet[n]; 
    for (size_t i = 0; i < n; i++){
        genSet[i] = false;
    }
    for (size_t i = 0; i < quorum; i++){
        int u = minDist(arrayDist,genSet,n); 
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
//funcion principal
tuple<float,int,int> coalicionGM(int m, float pmutacion_threshold, float pr)
{
	//llamada de JSON
	//inicializacion de semilla
	seed = time(0);
	mt.seed(seed);
	//para camara de diputados chile
	//ifstream archivo("ejemplo2.json");
	//json data = json::parse(archivo);

	//para parlamento de estados unidos
	ifstream archivo("ingles.json");
	json data = json::parse(archivo);


	//se crea y abre el archivo de salida
	//ofstream resultados;
	//resultados.open("resultados.json");

	//ofstream histo;
	//histo.open("hist.json");
	//string itera = "[";
	//string fitnessEvol = "[";

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
	for (size_t i = 0; i <n; i++)
	{

		//cout << i << endl;
		for (size_t j = 0; j <n; j++)
		{
			//cout << "i: " << i << endl;
			//cout << "j: " << j << endl;
			//cout << dis_euc(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]) << endl;
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


	//inicializacion de tiempo para calculo de ejecucion
	auto tInicial = chrono::high_resolution_clock::now();

	//inicializacion de quorum
	//para parlamento de estados unidos
	int quorum = trunc(n / 2) + 1;
	//para camara de diputados chile
	//int quorum = 74;



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

    for(int j = 0; j < n; j++){
        minDistEdge(cromosoma_n[j],matDis[j],n,quorum);
        sort_bubble(cromosoma_n[j],quorum);
        fitnessPobInit[j] = eval_sol(cromosoma_n[j],matDis,quorum);
    }

	/// Ordena los resultados
    sort_BubbleIndex(fitnessPobInitIndex,fitnessPobInit,n,n);
    // Traspasa los m mejores de toda la población inicial
    for(int i = 0; i <m; i++){
        memcpy(cromosoma[i],cromosoma_n[fitnessPobInitIndex[i]],sizeof(int)*quorum);
        fitnessPob[i] = fitnessPobInit[fitnessPobInitIndex[i]];
    }

	
	/////////////////////////////////////////////////////////////
	/////// Fin poblacion inicial
	////////////////////////////////////////////////////////////


	//inicializacion de variables de probabilidad
	float* p = (float*)malloc(m * sizeof(float));
	float* cump = (float*)malloc(m * sizeof(float));

	p[0] = 0.1;
	cump[0] = 0.1;

	//calculo de probabilidad
	for (size_t i = 1; i < m; i++)
	{
		p[i] = pr * pow(1 - pr, i);
		cump[i] = suma(p, i);
	}
	cump[m - 1] = 1;
	
	//inicializador de variables
	//int max_k = 10 * (m - 21);
	int max_k = 10*trunc((n+quorum)/m);
	int i = 0;
	int k = 0;
	int it = 0;

	//itera y fitnessEvol hay que almacenarlos en un archivo
	int cual1;
	int cual2;

	float pmutacion;
	bool flag2;
	bool flag3;

	int idxpop;
	int contCromNuevo = 0;

	int peor;

	float fitnessCambio;

	int min;

	//inicializador de variables de las nuevas poblaciones y sus respectivos fitness
	int** cromosomaNuevo = (int**)malloc(m * sizeof(int*));
	for (size_t a = 0; a < m; a++)
	{
		cromosomaNuevo[a] = (int*)malloc(quorum * sizeof(int));
	}
	float* fitnessPobNuevo = (float*)malloc(m * sizeof(float));

	int fitnessAnt;


	// Reservas extras 

	bool* aBoolean1 = nullptr;
	bool* aBoolean2 = nullptr;
	int** mDis12 = nullptr;
	int** mDis21 = nullptr;
	int* disimilar12 = nullptr;
	int* disimilar21 = nullptr;
	int* arrMin = nullptr;
	int* crossovern = nullptr;
	int* selecCrossover12 = nullptr;
	int* selecCrossover21 = nullptr;
	int* crossover12 = nullptr;
	int* crossover21 = nullptr;
	int** mCual12 = nullptr;
	int** mCual21 = nullptr;
	int* cromosoma1 = nullptr;
	int* cromosoma2 = nullptr;

    float bestSolution = 0.f;
	//incio de algoritmo
	while (k < max_k)
	{
		//guardamos fitness e iteraciones
		it++;
		fitnessAnt = fitnessPob[0];

		//se seleciona cromosomas a cruzar
		cual1 = smallest_greater(cump, m, (float)uni2(mt));
		cual2 = smallest_greater(cump, m, (float)uni2(mt));
		//estos no pueden ser iguales
		while (cual1 == cual2)cual2 = smallest_greater(cump, m, (float)uni2(mt));

		//se guardan los cromosomas a cruzar
		int* cromosoma1 = (int*)malloc(quorum * sizeof(int));
		int* cromosoma2 = (int*)malloc(quorum * sizeof(int));
		memcpy(cromosoma1, cromosoma[cual1], quorum * sizeof(int));
		memcpy(cromosoma2, cromosoma[cual2], quorum * sizeof(int));

		//extraccion de los genes a cruzar
		mDis12 = notin(cromosoma1, cromosoma2, quorum, quorum);		//Error alloc'd
		mDis21 = notin(cromosoma2, cromosoma1, quorum, quorum);

		int* disimilar12 = (int*)malloc(mDis12[1][0] * sizeof(int));
		int* disimilar21 = (int*)malloc(mDis21[1][0] * sizeof(int));

		memcpy(disimilar12, mDis12[0], mDis12[1][0] * sizeof(int));
		memcpy(disimilar21, mDis21[0], mDis21[1][0] * sizeof(int));

		//guardamos la cantidad de genes a cruzar
		int* arrMin = (int*)malloc(2 * sizeof(int));
		arrMin[0] = mDis12[1][0];
		arrMin[1] = mDis21[1][0];

		//seleccion de cantidad de genes a cruzar
		int* crossovern = (int*)malloc(sizeof(int));

		min = minimo(arrMin, 2);
		sample(crossovern, min, 1);

		//creamos y rellenamos los arrays de seleccion de genes a cruzar
		int* selecCrossover12 = (int*)malloc(crossovern[0] * sizeof(int));
		int* selecCrossover21 = (int*)malloc(crossovern[0] * sizeof(int));

		sample(selecCrossover12, mDis12[1][0], crossovern[0]);
		sample(selecCrossover21, mDis21[1][0], crossovern[0]);

		//guardamos los genes a cruzar
		int* crossover12 = (int*)malloc(crossovern[0] * sizeof(int));
		int* crossover21 = (int*)malloc(crossovern[0] * sizeof(int));

		for (size_t a = 0; a < crossovern[0]; a++)
		{
			crossover12[a] = disimilar12[selecCrossover12[a]];		//Error de lectura de tamaño 4
			crossover21[a] = disimilar21[selecCrossover21[a]];
		}

		//ordenamos los arreglos de los genes a cruzar
		sort_bubble(crossover12, crossovern[0]);
		sort_bubble(crossover21, crossovern[0]);

		//revisamos si los genes seleccionados se encuentran en el cromosoma aislado
		aBoolean1 = in_boolean(cromosoma1, crossover12, quorum, crossovern[0]);		//Error de escritura

		aBoolean2 = in_boolean(cromosoma2, crossover21, quorum, crossovern[0]);

		int** mCual12 = which(aBoolean1, quorum);
		int** mCual21 = which(aBoolean2, quorum);

		free(aBoolean1);
		free(aBoolean2);

		int* cual12 = nullptr;
		int* cual21 = nullptr;
		//en base a la posicion de genes sacada anteriormente ahora si realizamos los cruces a los cromosomas aislados
		cual12 = mCual12[0];
		cual21 = mCual21[0];
		for (size_t a = 0; a < mCual12[1][0]; a++)
		{
			cromosoma1[cual12[a]] = crossover21[a];
		}
		for (size_t a = 0; a < mCual21[1][0]; a++)
		{
			cromosoma2[cual21[a]] = crossover12[a];
		}
		//se ordenan
		sort_bubble(cromosoma1, quorum);
		sort_bubble(cromosoma2, quorum);

		//liberacion de memoria
		free(arrMin);
		free(mDis12[0]);
		free(mDis12[1]);
		free(mDis12);
		free(mDis21[0]);
		free(mDis21[1]);
		free(mDis21);
		free(mCual12[0]);
		free(mCual12[1]);
		free(mCual12);
		free(mCual21[0]);
		free(mCual21[1]);
		free(mCual21);
		free(disimilar12);
		free(disimilar21);
		free(crossover12);
		free(crossover21);
		free(selecCrossover12);
		free(selecCrossover21);
		//creamos la probabilidad de mutacion
		pmutacion = (float)uni2(mt);
		//comprobamos si se muta el cromosoma1
		if (pmutacion < pmutacion_threshold) {
			//seleccion de genes a mutar
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum, 1);
			int** mNotInCromosoma1 = nullptr;
			int* ar = crear_arreglo(n);
			mNotInCromosoma1 = notin(ar, cromosoma1, n, quorum);
			int* notInCromosoma1 = nullptr;
			notInCromosoma1 = mNotInCromosoma1[0];
			int* cualIntroducir = (int*)malloc(sizeof(int));
			sample_arreglo(cualIntroducir, 1, notInCromosoma1, mNotInCromosoma1[1][0]);
			//se realiza la mutacion y se ordena
			cromosoma1[cualSacar[0]] = cualIntroducir[0];
			sort_bubble(cromosoma1, quorum);
			//liberacion de memoria
			free(ar);
			free(cualSacar);
			free(mNotInCromosoma1[0]);
			free(mNotInCromosoma1[1]);
			free(mNotInCromosoma1);
			free(cualIntroducir);

		}
		//creamos la probabilidad de mutacion
		pmutacion = (float)uni2(mt);
		//comprobamos si se muta el cromosoma2
		if (pmutacion < pmutacion_threshold) {
			//seleccion de genes a mutar
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum, 1);
			int** mNotInCromosoma2 = nullptr;
			int* ar = crear_arreglo(n);
			mNotInCromosoma2 = notin(ar, cromosoma2, n, quorum);
			int* notInCromosoma2 = nullptr;
			notInCromosoma2 = mNotInCromosoma2[0];
			int* cualIntroducir = (int*)malloc(sizeof(int));
			sample_arreglo(cualIntroducir, 1, notInCromosoma2, mNotInCromosoma2[1][0]);
			//se realiza la mutacion y se ordena
			cromosoma2[cualSacar[0]] = cualIntroducir[0];
			sort_bubble(cromosoma2, quorum);
			//liberacion de memoria
			free(ar);
			free(cualSacar);
			free(mNotInCromosoma2[0]);
			free(mNotInCromosoma2[1]);
			free(mNotInCromosoma2);
			free(cualIntroducir);

		}
		//comprobacion si es la primera iteracion
		if (i == 0) {
			//inicializamos la nueva poblacion con los cromosomas aislados y recalculamos su fitness
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma1, quorum * sizeof(int));
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma1, matDis, quorum);
			contCromNuevo++;
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma2, quorum * sizeof(int));

			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma2, matDis, quorum);
			contCromNuevo++;
		}
		//comprobacion si no es la primera iteracion
		else {
			//inicializador de bandera y contador
			flag2 = true;
			idxpop = 0;
			while (flag2) {
				aBoolean1 = nullptr;
				//guardamos un arreglo de los booleanos si encuentra genes del cromosoma1 en el arreglo de cromosomas en la posición idxpop
				aBoolean1 = in_boolean(cromosoma1, cromosomaNuevo[idxpop], quorum, quorum);
				//si la suma de estos es igual al quorum
				if (suma_bool(aBoolean1, quorum) == quorum) {
					int* cualSacar = (int*)malloc(sizeof(int));
					// elegimos un gen aleatorio del para sacarlo
					sample(cualSacar, quorum, 1);
					int** mNotInCromosoma1 = nullptr;
					//creamos un arreglo
					int* ar = crear_arreglo(n);
					//selecciona los elementos que no estan en cromosoma1
					mNotInCromosoma1 = notin(ar, cromosoma1, n, quorum);
					int* notInCromosoma1 = nullptr;
					notInCromosoma1 = mNotInCromosoma1[0];
					int* cualIntroducir = (int*)malloc(sizeof(int));
					//selecciona un arreglo de largo 1 de elementos que no estan en cromosoma1 de manera aleatoria
					sample_arreglo(cualIntroducir, 1, notInCromosoma1, mNotInCromosoma1[1][0]);
					//se realiza la mutacion y se ordena
					cromosoma1[cualSacar[0]] = cualIntroducir[0];
					sort_bubble(cromosoma1, quorum);

					//limpiar memoria
					free(ar);
					free(cualSacar);
					free(mNotInCromosoma1[0]);
					free(mNotInCromosoma1[1]);
					free(mNotInCromosoma1);
					free(cualIntroducir);
					//resetear contador
					idxpop = 0;
				}
				else
				{
					//si ya recorrimos toda la poblacion nueva
					if (idxpop >= contCromNuevo - 1)
					{
						//salimos del ciclo
						flag2 = false;
					}
					else
					{
						//de lo contrario incrementamos el contador
						idxpop++;
					}
				}
				//liberacion de memoria
				free(aBoolean1);
			}
			//lo mismo explicado anteriormente pero para el cromosoma2
			flag2 = true;
			idxpop = 0;
			while (flag2) {
				aBoolean2 = nullptr;
				aBoolean2 = in_boolean(cromosoma2, cromosomaNuevo[idxpop], quorum, quorum);
				if (suma_bool(aBoolean2, quorum) == quorum) {
					int* cualSacar = (int*)malloc(sizeof(int));
					sample(cualSacar, quorum, 1);
					int** mNotInCromosoma2 = nullptr;
					int* ar = crear_arreglo(n);
					mNotInCromosoma2 = notin(ar, cromosoma2, n, quorum);
					int* notInCromosoma2 = nullptr;
					notInCromosoma2 = mNotInCromosoma2[0];
					int* cualIntroducir = (int*)malloc(sizeof(int));
					sample_arreglo(cualIntroducir, 1, notInCromosoma2, mNotInCromosoma2[1][0]);
					cromosoma2[cualSacar[0]] = cualIntroducir[0];
					sort_bubble(cromosoma2, quorum);

					free(ar);
					free(cualSacar);
					free(mNotInCromosoma2[0]);
					free(mNotInCromosoma2[1]);
					free(mNotInCromosoma2);
					free(cualIntroducir);

					idxpop = 0;
				}
				else
				{
					if (idxpop == contCromNuevo - 1)
					{
						flag2 = false;
					}
					else
					{
						idxpop++;
					}
				}
				free(aBoolean2);
			}

			//guardamos los cambios en el arreglo de cromosomas nuevos
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma1, quorum * sizeof(int));
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma1, matDis, quorum);
			contCromNuevo++;
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma2, quorum * sizeof(int));
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma2, matDis, quorum);		//Error salida de rango en llamada
			contCromNuevo++;

		}
		//aumentamos contador de iteraciones
		i++;
		//si llegamos a la mitad de la poblacion
		if (i == m / 2)
		{
			//reseteamos contador
			i = 0;
			contCromNuevo = 0;
			//ordenamos la poblacion nueva
			order(fitnessPobNuevo, cromosomaNuevo, quorum, m);
			
			//creacion de bandera
			flag2 = true;
			flag3 = false;
			idxpop = 0;

			while (flag2)
			{
				aBoolean1 = nullptr;
				//guardamos un arreglo de los booleanos si encuentra genes del cromosoma1 en el arreglo de cromosomas en la posición idxpop
				aBoolean1 = in_boolean(cromosoma[0], cromosomaNuevo[idxpop], quorum, quorum);
				//si la suma de estos es igual al quorum
				if (suma_bool(aBoolean1, quorum) == quorum)
				{
					//cambiamos las banderas
					flag2 = false;
					flag3 = true;
				}
				else
				{
					//de lo contrario incrementamos el contador
					idxpop++;
				}
				//si el contador indexpop llega al tamaño de la poplacion
				if (idxpop > m - 1)
				{
					//detenemos el ciclo
					flag2 = false;
				}
				//liberamos memoria
				free(aBoolean1);
			}
			float* pNuevo = (float*)malloc(m * sizeof(float));
			float* cumpNuevo = (float*)malloc(m * sizeof(float));
			//si despues de terminar el while anterior la bandera flag3 es false
			if (flag3 == false)
			{
				//cambiamos el peor cromosoma de la poblacion actual por el mejor cromosoma de la poblacion nueva
				peor = m - 1;
				memcpy(cromosomaNuevo[peor], cromosoma[0], quorum * sizeof(int));
				fitnessPobNuevo[peor] = fitnessPob[0];
				order(fitnessPobNuevo, cromosomaNuevo, quorum, m);
				memcpy(pNuevo, p, m * sizeof(float));
				memcpy(cumpNuevo, cump, m * sizeof(float));

			}
			else
			{
				//de lo contrario mantenemos la probabilidad y la probabilidad acumulada
				memcpy(pNuevo, p, m * sizeof(float));
				memcpy(cumpNuevo, cump, m * sizeof(float));
			}
			//cambiamos la poblacion actual por la poblacion nueva
			for (size_t a = 0; a < m; a++)
			{
				memcpy(cromosoma[a], cromosomaNuevo[a], quorum * sizeof(int));
				fitnessPob[a] = fitnessPobNuevo[a];
				p[a] = pNuevo[a];
				cump[a] = cumpNuevo[a];
			}
			//reemplazamos la mitad de la poblacion por nuevos cromosomas
			int* cromosomaCambio = (int*)malloc(quorum * sizeof(int));
			for (size_t j = m / 2; j < m; j++)
			{
				sample(cromosomaCambio, n, quorum);
				sort_bubble(cromosomaCambio, quorum);
				fitnessCambio = eval_sol(cromosomaCambio, matDis, quorum);
				memcpy(cromosoma[j], cromosomaCambio, quorum * sizeof(int));
				fitnessPob[j] = fitnessCambio;
			}
			//ordenamos la poblacion
			order(fitnessPob, cromosoma, quorum, m);
			//inicializamos la probabilidad y la probabilidad acumulada
			p[0] = pr;
			cump[0] = pr;
			//terminamos de rellenar la probabilidad y la probabilidad acumulada
			for (size_t j = 1; j < m; j++)
			{
				p[j] = pr * pow(1 - pr, j);
				cump[j] = suma(p, j);
			}
			//dejamos la ultima probabilidad acumulada en 1
			cump[m - 1] = 1;
			//reinicializamos el contador de iteraciones
			i = 0;

			
			k++;
			//si el fitness del mejor cromosoma es igual al fitness del mejor cromosoma anterior guardamos el numero de iteraciones y el fitness
			if (fitnessPob[0] < fitnessAnt) {
				//itera = itera + to_string(it) + ",";
				//fitnessEvol = fitnessEvol + to_string(fitnessPob[0]) + ",";
				k = 0;
			}
			//guardamos el fitness del mejor cromosoma
			fitnessAnt = fitnessPob[0];
			//liberamos memoria
			free(pNuevo);
			free(cumpNuevo);
			free(cromosomaCambio);
		}
		//liberamos memoria
		free(cromosoma1);
		free(cromosoma2);
		free(crossovern);
	}
	//itera = itera + to_string(it) + ",";
	//fitnessEvol = fitnessEvol + to_string(fitnessPob[0]) + ",";
	//cerramos el arreglo dentro del archivo json
	//replace(itera.end() - 1, itera.end(), ',', ']');
	//replace(fitnessEvol.end() - 1, fitnessEvol.end(), ',', ']');
	//calculo de tiempo
	auto tFinal = chrono::high_resolution_clock::now();
	double tTomado = chrono::duration_cast<chrono::nanoseconds>(tFinal - tInicial).count();
	tTomado *= 1e-9;

    bestSolution = fitnessPob[0];
	//guardamos la informacion recopilada en un json
	//resultados << "{\n\"m\": " << m << ",\n";
	//resultados << "\"pmutacion_threshold\": " << pmutacion_threshold << ",\n";
	//resultados << "\"pr\": " << pr << ",\n";
	//resultados << "\"seed\": " << seed << ",\n";
	//resultados << "\"numero_de_iteraciones\": " << it << ",\n";
	//resultados << "\"fitness\": " << fitnessPob[0] << ",\n";
	//resultados << "\"tiempo\": " << fixed << tTomado << setprecision(9) << ",\n";
	//resultados << "\"coalicion\": [";

	//for (size_t j = 0; j < quorum; j++)
	//{
		//para el parlamento Chileno
		//if(j<quorum-1)resultados << data["diputados"][cromosoma[0][j]]["ID"] << ",";
		//else resultados << data["diputados"][cromosoma[0][j]]["ID"];

		//para el parlamento de estados unidos
		//if (j < quorum - 1)resultados << cromosoma[0][j] << ",";
		//else resultados << cromosoma[0][j];
	//}
	//resultados << "]\n" << "}";
	//resultados.close();
	
	//guardamos datos para el histograma
	//histo << "{\n\"iteraciones\": " << itera << ",\n";
	//histo << "\"fitness\": " << fitnessEvol << "\n}";
	
	//liberamos memoria
	free(fitnessPob);
	free(fitnessPobNuevo);
	free(p);
	free(cump);
	for (size_t j = 0; j < m; j++)
	{
		free(cromosomaNuevo[j]);
		free(cromosoma[j]);
	}
	for (size_t i = 0; i < n; i++)
	{
		free(matDis[i]);
	}
	free(cromosomaNuevo);
	free(cromosoma);
	free(matDis);
	return make_tuple(bestSolution,it,seed);
}