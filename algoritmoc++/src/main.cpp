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
// srand() para usar la semilla
using namespace std;
using json = nlohmann::json;

int m = 40;
float pmutacion_threshold = 0.2;
float pr = 0.1;
int seed = time(NULL);

random_device rng;
default_random_engine generator(seed);
mt19937 mt{ rng() };

uniform_int_distribution<int> uni;
uniform_real_distribution<double> uni2;

float dis_euc(float x1, float y1, float x2, float y2)
{
	float calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (float)2);
	return calculo;
}

float eval_sol(int* pos,float** mat,int largo) {
	float suma = 0;
	//cout << largo << endl << endl;
	for (size_t i = 0; i <= (largo - 2); i++)
	{
		
		//cout << i << endl;
		for (size_t j = i + 1; j <= (largo-1); j++)
		{
			suma = suma + mat[pos[i]][pos[j]];
		}
	}
	return suma;
}

float eval_sol2(int* pos, json data, int largo) {
	float suma = 0;
	//cout << largo << endl << endl;
	for (size_t i = 0; i <= (largo - 2); i++)
	{
		for (size_t j = i + 1; j <= largo-1; j++)
		{	
			//cout << "i: " << i << "j: " << j << endl;
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
	uni = uniform_int_distribution<int> (0, limite - 1);
	while (i < largo)
	{
		//cout << uni(mt)<<endl;
		//valor = rand() % limite;
		valor = uni(mt);
		/*if (limite - 1 == 0) {
			valor = 0;
		}
		else {
			valor = rand() % (limite - 1);
		}
		*/
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
	//free(arrTemp);
}

int **notin(int *array1, int *array2, int largo1,int largo2)
{
	int *temp = (int *)malloc(sizeof(int) * largo1);
	int cont = 0;
	bool flag = false;
	for (int i = 0; i < largo1; i++)
	{
		for (int j = 0; j < largo2; j++)
		{
			/*cout << "Arreglo 1=" << array1[i] << endl;
			cout << "Arreglo 2=" << array2[j] << endl;*/
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
	//cout << cont << endl;
	int *result = (int *)malloc(sizeof(int) * cont);
	for (size_t i = 0; i < cont; i++)
	{
		result[i] = temp[i];
	}
	//delete[] temp; // revisar si esta bien
	int **ret = (int **)malloc(sizeof(int *) * 2);
	int *pointCont = (int *)malloc(sizeof(int) * 1);
	pointCont[0] = cont;
	ret[0] = result;
	ret[1] = pointCont;
	return ret;
}

bool *in_boolean(int *array1, int *array2, int largo1, int largo2)
{
	bool *result = (bool *)malloc(sizeof(bool) * largo1);
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

int which_max(float *array, int largo)
{
	int maxi = array[0];
	//cout << largo << endl;
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
		//cout << i << " ";
		if (array[i] == maxi)
			return i;
	}
	//cout << endl;
}

int **which(bool *array, int largo)
{
	int* result = nullptr;
	int **ret = (int **)malloc(sizeof(int *) * 2);
	int cont = 0;
	//cout << "arreglo:";
	for (size_t i = 0; i < largo-1; i++)
	{
		//cout << array[i] << " ";
		if (array[i])
			cont++;
	}
	//cout << endl;
	result = (int *)malloc(sizeof(int) * cont);
	cont = 0;
	for (size_t i = 0; i < largo; i++)
	{
		//cout << "a"<<endl;
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
	//free(result);
	//free(pointerCont);

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
	//cout << newlargo << endl;
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
	uni = uniform_int_distribution<int> (0, largo - 1);
	while (i < cant)
	{
		//cout << uni(mt) << endl;
		indice = uni(mt);
		//cout << largo << endl;
		//indice = rand() % (largo - 1);
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
	int n = data["diputados"].size();

	float** matDis = (float**)malloc(n * sizeof(float*));
	for (size_t i = 0; i < n; i++)
	{
		matDis[i] = (float*)malloc(n * sizeof(float));
	}


	for (size_t i = 0; i <= (n - 2); i++)
	{

		//cout << i << endl;
		for (size_t j = i + 1; j <= (n - 1); j++)
		{
			matDis[i][j] = dis_euc(data["diputados"][i]["coordX"], data["diputados"][i]["coordY"], data["diputados"][j]["coordX"], data["diputados"][j]["coordY"]);
		}
	}
	auto tInicial = chrono::high_resolution_clock::now();
	//srand(seed);
	int quorum = 74;
	if (argc > 1)
	{
		m = stoi(argv[0]);
		pmutacion_threshold = stod(argv[1]);
		pr = stod(argv[2]);
		seed = stoi(argv[3]);
	}
	mt.seed(seed);
	
	uni2 = uniform_real_distribution<double>(0, 1);

	int **cromosoma = (int **)malloc(m * sizeof(int *));
	float* fitnessPob = (float*)malloc(m * sizeof(float));

	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int *)malloc(quorum * sizeof(int));
	}

	 //cout << "primero --------------------" << endl;

	for (size_t i = 0; i < m; i++)
	{
		sample(cromosoma[i],n, quorum);
		sort(cromosoma[i], quorum);
		fitnessPob[i] = eval_sol(cromosoma[i],matDis,quorum);
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
	int contCromNuevo =0;

	int peor;

	float fitnessCambio;

	int min;

	int** cromosomaNuevo = (int**)malloc(m * sizeof(int*));
	float* fitnessPobNuevo = (float*)malloc(m * sizeof(float));
	while (k<max_k)
	{

		//cout << "iteracion: " << it+1 << endl;
		//cout << rand() / static_cast<float>(RAND_MAX) << endl;
		it++;
		//cout << mt << endl;
		cual1 = smallest_greater(cump, m, (float)uni2(mt));
		cual2 = smallest_greater(cump, m, (float)uni2(mt));
		//cual1 = smallest_greater(cump, m, rand() / static_cast<float>(RAND_MAX));
		//cual2 = smallest_greater(cump, m, rand() / static_cast<float>(RAND_MAX));
		while (cual1 == cual2)cual2 = smallest_greater(cump, m, (float)uni2(mt));
		//while (cual1 == cual2)cual2 = smallest_greater(cump, m, rand() / static_cast<float>(RAND_MAX));

		int* cromosoma1=(int*)malloc(quorum*sizeof(int));
		int* cromosoma2 = (int*)malloc(quorum * sizeof(int));
		memcpy(cromosoma1, cromosoma[cual1], quorum * sizeof(int));
		memcpy(cromosoma2, cromosoma[cual2], quorum * sizeof(int));

		int** mDis12 = (int**)malloc(2 * sizeof(int*));
		int** mDis21 = (int**)malloc(2 * sizeof(int*));

		mDis12 = notin(cromosoma1, cromosoma2, quorum,quorum);
		mDis21 = notin(cromosoma2, cromosoma1, quorum,quorum);

		int* disimilar12 = (int*)malloc(mDis12[1][0] * sizeof(int));
		int* disimilar21 = (int*)malloc(mDis21[1][0] * sizeof(int));

		memcpy(disimilar12, mDis12[0], mDis12[1][0] * sizeof(int));
		memcpy(disimilar21, mDis21[0], mDis21[1][0] * sizeof(int));

		int* arrMin = (int*)malloc(2 * sizeof(int));
		arrMin[0] = mDis12[1][0];
		arrMin[1] = mDis21[1][0];

		int* crossovern = (int*)malloc(sizeof(int));

		min = minimo(arrMin, 2);
		sample(crossovern, min, 1);

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
		//revisar which y in_boolean.
		mCual12 = which(in_boolean(cromosoma1, crossover12, quorum, crossovern[0]), quorum);
		mCual21 = which(in_boolean(cromosoma2, crossover21, quorum, crossovern[0]), quorum);
		
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
		
		free(mDis12);
		free(mDis21);
		free(mCual12);
		free(mCual21);
		
		pmutacion = (float)uni2(mt);
		//pmutacion = rand() / static_cast<float>(RAND_MAX);
		if (pmutacion < pmutacion_threshold) {
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum,1);
			int** mNotInCromosoma1 = (int**)malloc(2 * sizeof(int*));
			mNotInCromosoma1 = notin(crear_arreglo(n), cromosoma1, n,quorum);
			//cout << mNotInCromosoma1[1][0] << endl;
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

		pmutacion = (float)uni2(mt);
		//pmutacion = rand() / static_cast<float>(RAND_MAX);
		if (pmutacion < pmutacion_threshold) {
			int* cualSacar = (int*)malloc(sizeof(int));
			sample(cualSacar, quorum, 1);
			int** mNotInCromosoma2 = (int**)malloc(2 * sizeof(int*));
			mNotInCromosoma2 = notin(crear_arreglo(n), cromosoma2, n,quorum);
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

		if (i == 0) {
			
			for (size_t a = 0; a < m; a++)
			{
				cromosomaNuevo[a] = (int*)malloc(quorum * sizeof(int));
			}

			memcpy(cromosomaNuevo[contCromNuevo], cromosoma1, quorum * sizeof(int));
			//cromosomaNuevo[contCromNuevo] = cromosoma1;
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma1, matDis, quorum);
			contCromNuevo++;
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma2, quorum * sizeof(int));
			
			//cromosomaNuevo[contCromNuevo] = cromosoma2;
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma2, matDis, quorum);
			contCromNuevo++;
		}
		else {
			flag2 = true;
			idxpop = 0;
			while (flag2) {
				if (suma_bool(in_boolean(cromosoma1, cromosomaNuevo[idxpop], quorum,quorum), quorum) == quorum) {
					int* cualSacar = (int*)malloc(sizeof(int));
					sample(cualSacar, quorum, 1);
					int** mNotInCromosoma1 = (int**)malloc(2 * sizeof(int*));
					mNotInCromosoma1 = notin(crear_arreglo(n), cromosoma1, n,quorum);
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
					
					idxpop = 0;
				}
				else
				{
					if (idxpop>=contCromNuevo-1)
					{
						flag2 = false;
					}
					else
					{
						idxpop++;
					}
				}
			}

			flag2 = true;
			idxpop = 0;
			while (flag2) {
				
				if (suma_bool(in_boolean(cromosoma2, cromosomaNuevo[idxpop], quorum,quorum), quorum) == quorum) {
					/*if (it >= 249) {
						cout << "cromosoma nuevo" << endl;
						for (size_t j = 0; j < quorum; j++)
						{
							cout << cromosomaNuevo[idxpop][j] << " ";
						}
						cout << endl;
						cout << "cromosoma2" << endl;
						for (size_t j = 0; j < quorum; j++)
						{
							cout << cromosoma2[j] << " ";
						}
						cout << endl;
					}*/
					int* cualSacar = (int*)malloc(sizeof(int));
					sample(cualSacar, quorum, 1);
					int** mNotInCromosoma2 = (int**)malloc(2 * sizeof(int*));
					mNotInCromosoma2 = notin(crear_arreglo(n), cromosoma2, n,quorum);
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
			}
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma1, quorum * sizeof(int));
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma1, matDis, quorum);
			contCromNuevo++;
			memcpy(cromosomaNuevo[contCromNuevo], cromosoma2, quorum * sizeof(int));
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma2, matDis, quorum);
			contCromNuevo++;

		}
		i++;
		if (i==m/2)
		{
			i = 0;
			contCromNuevo = 0;
			order(fitnessPobNuevo, cromosomaNuevo, quorum, m);

			flag2 = true;
			flag3 = false;
			idxpop = 0;

			while (flag2)
			{
				if (suma_bool(in_boolean(cromosoma[0], cromosomaNuevo[idxpop], quorum,quorum), quorum) == quorum)
				{
					flag2 = false;
					flag3 = true;
				}
				else
				{
					idxpop++;
				}
				if (idxpop>m-1)
				{
					flag2 = false;
				}
			}
			float* pNuevo = (float*)malloc(m * sizeof(float));
			float* cumpNuevo = (float*)malloc(m * sizeof(float));
			if (flag3 == false)
			{
				/*for (size_t j = 0; j < m; j++)
				{
					cout << "j: " << j << " " << fitnessPobNuevo[j] << " ";
				}
				cout << endl;*/
				//peor = which_max(fitnessPobNuevo, m);
				//cout << peor << endl;
				peor = m-1;
				memcpy(cromosomaNuevo[peor], cromosoma[0], quorum * sizeof(int));
				fitnessPobNuevo[peor] = fitnessPob[0];
				order(fitnessPobNuevo, cromosomaNuevo, quorum, m);

				//aqui va lo que no se deberia tener que hacer
				memcpy(pNuevo, p, m * sizeof(float));
				memcpy(cumpNuevo, cump, m * sizeof(float));

				if (fitnessPobNuevo[0] == fitnessPob[0])
				{
					k++;
				}

			}
			else
			{
				memcpy(pNuevo, p, m * sizeof(float));
				memcpy(cumpNuevo, cump, m * sizeof(float));

				if (fitnessPobNuevo[0] == fitnessPob[0])
				{
					k++;
				}
			}
			for (size_t a = 0; a < m; a++)
			{
				memcpy(cromosoma[a], cromosomaNuevo[a], quorum * sizeof(int));
				//revisar
				fitnessPob[a] = fitnessPobNuevo[a];
				p[a] = pNuevo[a];
				cump[a] = cumpNuevo[a];
			}
			/*for (size_t j = 0; j < m; j++)
			{
				free(cromosomaNuevo[j]);
			}
			free(cromosomaNuevo);
			free(fitnessPobNuevo);
			free(pNuevo);
			free(cumpNuevo);*/
			

			int* cromosomaCambio = (int*)malloc(quorum * sizeof(int));
			for (size_t j = round(m/2); j < m; j++)
			{
				sample(cromosomaCambio, n, quorum);
				sort(cromosomaCambio, quorum);
				fitnessCambio = eval_sol(cromosomaCambio, matDis, quorum);
				memcpy(cromosoma[j], cromosomaCambio, quorum * sizeof(int));
				fitnessPob[j] = fitnessCambio;
			}
			order(fitnessPob, cromosoma, quorum, m);
			
			p[0] = pr;
			cump[0] = pr;

			for (size_t j = 1; j < m; j++)
			{
				p[j] = pr * pow(1 - pr, j);
				cump[j] = suma(p, j);
			}
			cump[m - 1] = 1;
			i = 0;
		}

	}
	auto tFinal = chrono::high_resolution_clock::now();
	double tTomado = chrono::duration_cast<chrono::nanoseconds>(tFinal-tInicial).count();
	tTomado *= 1e-9;
	cout << fitnessPob[0] << "it: " << it << endl;
	cout << "T=" << fixed<<tTomado<<setprecision(9) << endl;
	for (size_t j = 0; j < quorum; j++)
	{
		cout << cromosoma[0][j] << " ";
	}
}