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

int m = 5;
float pmutacion_threshold = 0.2;
float pr = 0.1;
int seed = 20;

float eval_sol(float pos[], json data) {
	float suma = 0;
	for (size_t i = 0; i < (sizeof(pos) - 1); i++)
	{
		for (size_t j = i + 1; j < sizeof(pos); j++)
		{
			suma = suma + dis_euc(data["diputados"][pos[i]]["coordX"], data["diputados"][pos[i]]["coordY"], data["diputados"][pos[j]]["coordX"], data["diputados"][pos[j]]["coordY"]);
		}
	}
	return suma;
}

float dis_euc(float x1, float y1, float x2, float y2)
{
	float calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / 2);
	return calculo;
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
				return NULL;
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
		valor = rand() % limite;
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
		for (size_t j = 0; j < largo - 1; j++)
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
// revisar si esta bien (memcpy, para copiar un arreglo de una posicion a otra)
/*float* order(float* array[]) {
	int n = sizeof(array[1]);
	bool already_sorted = true;
	for (size_t i = 0; i < n; i++)
	{
		already_sorted = true;
		for (size_t j = 0; j < (n - i - 1); j++)
		{
			if (array[1][j] > array[1][j + 1])
			{
				array[0][j], array[0][j + 1] = array[0][j + 1], array[0][j];
				array[1][j], array[1][j + 1] = array[1][j + 1], array[1][j];
				already_sorted = false;
			}
		}
		if (already_sorted) break;
	}
	return (array[0], array[1]);
}*/

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
	int valor = 0;
	while (i < cant)
	{
		indice = rand() % largo;
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
	int quorum = 20;
	if (argc > 1)
	{
		m = stoi(argv[0]);
		pmutacion_threshold = stod(argv[1]);
		pr = stod(argv[2]);
		seed = stoi(argv[3]);
	}

	int **cromosoma = (int **)malloc(m * sizeof(int *));

	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int *)malloc(quorum * sizeof(int));
	}
}