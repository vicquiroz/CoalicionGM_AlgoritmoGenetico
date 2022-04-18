#include <iostream>
#include <cstdlib>
#include <fstream>
#include <random>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <nlohmann/json.hpp>

// srand() para usar la semilla
using namespace std;
using json = nlohmann::json;

int m = 40;
float pmutacion_threshold = 0.2;
float pr = 0.1;
unsigned seed = 1234;
random_device rng;
//default_random_engine generator(seed);
mt19937 mt(rng());

float dis_euc(float x1, float y1, float x2, float y2)
{
	float calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (float)2);
	return calculo;
}

float eval_sol(int* pos, json data,int largo) {
	float suma = 0;
	for (size_t i = 0; i < (largo - 1); i++)
	{
		for (size_t j = i + 1; j < largo; j++)
		{
			suma = suma + dis_euc(data["diputados"][pos[i]]["coordX"], data["diputados"][pos[i]]["coordY"], data["diputados"][pos[j]]["coordX"], data["diputados"][pos[j]]["coordY"]);
		}
	}
	return suma;
}

float suma(float *array, int largo)
{
	float suma = 0;
	for (size_t i = 0; i <= largo; i++)
	{
		suma = suma + array[i];
	}
	return suma;
}

int suma_bool(bool *array, int largo)
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
// ver return null (posible return -1)
int smallest_greater(float *seq, int largo, float value)
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
			if(i==largo)
			{
				EXIT_FAILURE;
			}
		}
	}
}

void sample(int *arreglo, int limite, int largo)
{
	int i = 0;
	bool repetido = false;
	int valor = 0;
	while (i < largo)
	{
		//valor = rand() % limite;
		uniform_int_distribution<int> uni(0, limite-1);
		int valor = uni(mt);
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

void sort(int *array, int largo)
{
	int temp = 0;
	for (size_t i = 0; i < largo; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < largo -i- 1; j++)
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

void order(float* fitness, int**cromosoma, int quorum, int m)
{
	int temp = 0;
	int* arrTemp = (int*)malloc(quorum * sizeof(int));
	for (size_t i = 0; i < m; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < m -i- 1; j++)
		{
			if (fitness[j] > fitness[j + 1])
			{
				temp = fitness[j];
				fitness[j] = fitness[j + 1];
				fitness[j + 1] = temp;

				memcpy(arrTemp, cromosoma[j], sizeof(int)*quorum);
				memcpy(cromosoma[j], cromosoma[j+1], sizeof(int)*quorum);
				memcpy(cromosoma[j+1], arrTemp, sizeof(int)*quorum);
				already_sorted = false;
			}
		}
		if (already_sorted)
			break;
		
	}
	free(arrTemp);
}

int **notin(int *array1, int *array2, int largo)
{
	int *temp = (int *)malloc(sizeof(int) * largo);
	int cont = 0;
	bool flag = false;
	for (int i = 0; i < largo; i++)
	{
		for (int j = 0; j < largo; j++)
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
	int *result = (int *)malloc(sizeof(int) * cont);
	for (size_t i = 0; i < cont; i++)
	{
		result[i] = temp[i];
	}
	delete[] temp; // revisar si esta bien
	int **ret = (int **)malloc(sizeof(int *) * 2);
	int *pointCont = (int *)malloc(sizeof(int) * 1);
	pointCont[0] = cont;
	ret[0] = result;
	ret[1] = pointCont;
	return ret;
}

bool *in_boolean(int *array1, int *array2, int largo)
{
	bool *result = (bool *)malloc(sizeof(bool) * largo);
	int result_len = 0;
	bool flag = false;
	for (size_t i = 0; i < largo; i++)
	{
		for (size_t j = 0; j < largo; j++)
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
int minimo(int *array, int largo)
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

int maximo(int *array, int largo)
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

int which_max(int *array, int largo)
{
	int maxi = 0;
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
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i] == maxi)
			return i;
	}
}

int **which(bool *array, int largo)
{
	int **ret = (int **)malloc(sizeof(int *) * 2);
	int cont = 0;
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i])
			cont++;
	}
	int *result = (int *)malloc(sizeof(int) * cont);
	cont = 0;
	for (size_t i = 0; i < largo; i++)
	{
		if (array[i])
		{
			result[cont] = i;
			cont++;
		}
	}
	int *pointerCont = (int *)malloc(sizeof(int) * 1);
	pointerCont[0] = cont;
	ret[0] = result;
	ret[1] = pointerCont;
	return ret;
}

int* crear_arreglo(int largo)
{
	int* arreglo = (int*)malloc(sizeof(int) * largo);
	for (size_t i = 0; i < largo; i++)
	{
		arreglo[i] = i;
	}

	return arreglo;
}

int** repetidos(int* arreglo, int largo)
{
	int cont = 0;
	int aux = 0;
	int newlargo = largo;
	int* newarr = (int*)malloc(sizeof(int) * largo);
	for (size_t i = 0; i < newlargo; i++)
	{
		newarr[i] = arreglo[i];
	}
	for (size_t i = 0; i < newlargo; i++)
	{
		for (size_t j = 0; j < newlargo; j++)
		{
			if (newarr[i] == newarr[j]) cont++;
		}
		if (cont > 1) {
			aux = i + 1;
			for (size_t j = i + 1; j < newlargo; j++)
			{
				if (newarr[j] != newarr[i]) newarr[aux] = newarr[j];
			}
			newlargo = newlargo - cont + 1;
			aux = 0;
			cont = 0;
		}
	}
	cout << newlargo << endl;
	int* finalarr = (int*)malloc(sizeof(int) * newlargo);
	for (size_t i = 0; i < newlargo; i++)
	{
		finalarr[i] = newarr[i];
	}
	delete[] newarr;
	int** ret = (int**)malloc(sizeof(int*) * 2);
	int* pointCont = (int*)malloc(sizeof(int) * 1);
	pointCont[0] = newlargo;
	ret[0] = finalarr;
	ret[1] = pointCont;
	return ret;
}

void sample_arreglo(int* arreglo, int cant, int* valores, int largo)
{
	int indice = 0;
	int valor = 0;
	int i = 0;
	bool repetido = false;
	while (i < cant)
	{
		uniform_int_distribution<int> uni(0, largo - 1);
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


void main(int argc, char *argv[])
{
	uniform_real_distribution<float> uni(0, 1);
	ifstream archivo("ejemplo2.json");
	json data = json::parse(archivo);

	int quorum = 74;
	if (argc > 1)
	{
		m = stoi(argv[0]);
		pmutacion_threshold = stod(argv[1]);
		pr = stod(argv[2]);
		seed = stoi(argv[3]);
	}

	mt.seed(seed);

	int **cromosoma = (int **)malloc(m * sizeof(int *));
	float* fitnessPob = (float*)malloc(m * sizeof(float));

	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int *)malloc(quorum * sizeof(int));
	}

	int n = data["diputados"].size();
	for (size_t i = 0; i < m; i++)
	{
		sample(cromosoma[i],n, quorum);
		sort(cromosoma[i], quorum);
		fitnessPob[i] = eval_sol(cromosoma[i],data,quorum);
	}
	
	order(fitnessPob, cromosoma, quorum, m);

	float* p = (float *)malloc(m * sizeof(float));
	float* cump = (float*)malloc(m * sizeof(float));

	p[0] = 0.1;
	cump[0] = 0.1;

	for (size_t i = 1; i < m; i++)
	{
		p[i] = pr*pow(1 - pr, i);
		cump[i] = suma(p,i);
	}
	cump[m - 1] = 1;
	int max_k = 10 * (m - 21);
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

	while (k<max_k)
	{
		it++;
		cual1 = smallest_greater(cump, m, uni(mt));
		cual2 = smallest_greater(cump, m, uni(mt));
		while (cual1 == cual2)cual2 = smallest_greater(cump, m, uni(mt));

		int* cromosoma1=(int*)malloc(quorum*sizeof(int));
		int* cromosoma2 = (int*)malloc(quorum * sizeof(int));
		memcpy(cromosoma1, cromosoma[cual1], quorum * sizeof(int));
		memcpy(cromosoma2, cromosoma[cual2], quorum * sizeof(int));

		int** mDis12 = notin(cromosoma1, cromosoma2, quorum);
		int** mDis21 = notin(cromosoma2, cromosoma1, quorum);

		int* disimilar12 = (int*)malloc(quorum * sizeof(int));
		int* disimilar21 = (int*)malloc(quorum * sizeof(int));

		memcpy(disimilar12, mDis12[0], quorum * sizeof(int));
		memcpy(disimilar21, mDis21[0], quorum * sizeof(int));

		int* arrMin = (int*)malloc(2 * sizeof(int));
		arrMin[0] = mDis12[1][0];
		arrMin[1] = mDis21[1][0];

		int* crossovern = (int*)malloc(sizeof(int));

		sample(crossovern, minimo(arrMin, 2), 1);

		int* selecCrossover12 = (int*)malloc(crossovern[0] * sizeof(int));
		int* selecCrossover21 = (int*)malloc(crossovern[0] * sizeof(int));

		sample(selecCrossover12, mDis12[1][0], crossovern[0]);
		sample(selecCrossover21, mDis21[1][0], crossovern[0]);

		int* crossover12 = (int*)malloc(crossovern[0] * sizeof(int));
		int* crossover21 = (int*)malloc(crossovern[0] * sizeof(int));

		for (size_t a = 0; a < crossovern[0]; a++)
		{
			crossover12[a] = disimilar12[selecCrossover12[a]];
			crossover21[a] = disimilar21[selecCrossover21[a]];
		}
		sort(crossover12, crossovern[0]);
		sort(crossover21, crossovern[0]);

		int** mCual12 = (int**)malloc(2 * sizeof(int*));
		int** mCual21 = (int**)malloc(2 * sizeof(int*));

		mCual12 = which(in_boolean(cromosoma1, crossover12, quorum), quorum);
		mCual21 = which(in_boolean(cromosoma2, crossover21, quorum), quorum);

		int* cual12 = (int*)malloc(mCual12[1][0] * sizeof(int));
		int* cual21 = (int*)malloc(mCual21[1][0] * sizeof(int));

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

		sort(cromosoma1, quorum);
		sort(cromosoma2, quorum);

		pmutacion = uni(mt);
		if (pmutacion < pmutacion_threshold) {
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum,1);
			int** mNotInCromosoma1 = (int**)malloc(2 * sizeof(int*));
			mNotInCromosoma1 = notin(crear_arreglo(n), cromosoma1, n);
			int* notInCromosoma1 = (int*)malloc(mNotInCromosoma1[1][0] * sizeof(int));
			notInCromosoma1 = mNotInCromosoma1[0];
			int* cualIntroducir = (int*)malloc(sizeof(int));
			sample_arreglo(cualIntroducir, 1, notInCromosoma1, mNotInCromosoma1[1][0]);
			cromosoma1[cualSacar[0]] = cualIntroducir[0];
			sort(cromosoma1, quorum);
			free(cualSacar);
			free(mNotInCromosoma1);
			free(notInCromosoma1);
			free(cualIntroducir);
		}

		pmutacion = uni(mt);
		if (pmutacion < pmutacion_threshold) {
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum, 1);
			int** mNotInCromosoma2 = (int**)malloc(2 * sizeof(int*));
			mNotInCromosoma2 = notin(crear_arreglo(n), cromosoma2, n);
			int* notInCromosoma2 = (int*)malloc(mNotInCromosoma2[1][0] * sizeof(int));
			notInCromosoma2 = mNotInCromosoma2[0];
			int* cualIntroducir = (int*)malloc(sizeof(int));
			sample_arreglo(cualIntroducir, 1, notInCromosoma2, mNotInCromosoma2[1][0]);
			cromosoma2[cualSacar[0]] = cualIntroducir[0];
			sort(cromosoma2, quorum);
			free(cualSacar);
			free(mNotInCromosoma2);
			free(notInCromosoma2);
			free(cualIntroducir);
		}
		int** cromosomaNuevo = (int**)malloc(m * sizeof(int*));
		float* fitnessPobNuevo = (float*)malloc(m * sizeof(float));
		if (i == 0) {
			for (size_t a = 0; a < m; a++)
			{
				cromosomaNuevo[a] = (int*)malloc(quorum * sizeof(int));
			}
			memcpy(cromosomaNuevo[0], cromosoma1, quorum * sizeof(int));
			memcpy(cromosomaNuevo[1], cromosoma2, quorum * sizeof(int));
			fitnessPobNuevo[0] = eval_sol(cromosoma1, data, quorum);
			fitnessPobNuevo[1] = eval_sol(cromosoma2, data, quorum);
		}
		else {
			flag2 = true;
			idxpop = 0;
			while (flag2) {
				if (suma_bool(in_boolean(cromosoma1, cromosomaNuevo[idxpop], quorum), quorum) == quorum) {
					int* cualSacar = (int*)malloc(sizeof(int));//linea 294 py
				}
			}
		}

	}
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < quorum; j++)
		{
			cout << cromosoma[i][j] << " ";
		}
		cout << endl;
	}
}