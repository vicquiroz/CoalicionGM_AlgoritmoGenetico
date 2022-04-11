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
int seed = 20;

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
	for (size_t i = 0; i < largo; i++)
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
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, limite-1);
		int valor = uni(rng);
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
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, largo - 1);
		indice = uni(rng);
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
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < quorum; j++)
		{
			cout << cromosoma[i][j] << " ";
		}
		cout <<endl<< fitnessPob[i] << endl<<endl;
	}
	cout << "-------------------------------------------------------------------------------------------------"<<endl;
	order(fitnessPob, cromosoma, quorum, m);

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < quorum; j++)
		{
			cout << cromosoma[i][j] << " ";
		}
		cout << endl << fitnessPob[i] << endl << endl;
	}
}