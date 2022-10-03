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
#include "functions.h"
//acortadores
using namespace std;
using json = nlohmann::json;

//parametros iniciales
int m = 38;
double pmutacion_threshold = 0.170002;
double pr = 0.141;

//funcion principal
int main(int argc, char* argv[])
{
	//llamada de JSON
	//para parlamento de estados unidos
	ifstream archivo("votes.json");
	json data = json::parse(archivo);


	//se crea y abre el archivo de salida
	ofstream resultados;
	resultados.open("resultados.json");

	ofstream histo;
	histo.open("hist.json");
	string coalicionEvol = "[\n";
	string itera = "[";
	string fitnessEvol = "[";
	
	//de los JSON se obtiene la cantidad de diputados
	//para parlamento de estados unidos
	int n = data["rollcalls"][0]["votes"].size();

	//para camara de diputados chile
	//int n = data["diputados"].size();

	//creacion de la matriz de distancia
	double** matDis = (double**)malloc(n * sizeof(double*));
	for (size_t i = 0; i < n; i++)
	{
		matDis[i] = (double*)malloc(n * sizeof(double));
	}

	//rellenado de la matriz de distancia
	// Matriz rellenando por completo
	for (size_t i = 0; i <n; i++)
	{

		//cout << i << endl;
		for (size_t j = 0; j <n; j++)
		{	
			matDis[i][j] = dis_euc(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]);
		}
	}

	//inicializacion de tiempo para calculo de ejecucion
	auto tInicial = chrono::high_resolution_clock::now();

	//inicializacion de quorum
	//para parlamento de estados unidos
	int quorum = trunc(n / 2) + 1;

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

	//generador de double's random entre 0 y 1
	uni2 = uniform_real_distribution<double>(0, 1);

	//inicializador de Poblacion y fitness
	int** cromosoma = (int**)malloc(m * sizeof(int*));
	double* fitnessPob = nullptr;
	fitnessPob = (double*)malloc(m * sizeof(double));

	//inicializacion de cromosomas
	for (size_t i = 0; i < m; i++)
	{
		cromosoma[i] = (int*)malloc(quorum * sizeof(int));
	}

	for (size_t i = 0; i < m; i++)
	{
		sample(cromosoma[i], n, quorum);
		//sort_bubble(cromosoma[i], quorum);
		sort(cromosoma[i], cromosoma[i] + quorum, &array_sort);
		fitnessPob[i] = eval_sol(cromosoma[i], matDis, quorum);
	}
	
	//ordenamiento de fitness y cromosomas
	order(fitnessPob, cromosoma, quorum, m);
	coalicionEvol = coalicionEvol + "[";
	for (size_t i = 0; i < quorum; i++)
	{
		coalicionEvol=coalicionEvol+to_string(cromosoma[0][i])+",";
	}
	replace(coalicionEvol.end() - 1, coalicionEvol.end(), ',', ']');
	coalicionEvol = coalicionEvol + ",";

	//inicializacion de variables de probabilidad
	double* p = (double*)malloc(m * sizeof(double));
	double* cump = (double*)malloc(m * sizeof(double));

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
	//int max_k = 10*trunc((n+quorum)/m);
	int max_k = 200 * trunc((n + quorum) / m);
	int i = 0;
	int k = 0;
	int it = 0;

	//itera y fitnessEvol hay que almacenarlos en un archivo
	int cual1;
	int cual2;

	double pmutacion;
	bool flag2;
	bool flag3;

	int idxpop;
	int contCromNuevo = 0;

	int peor;

	double fitnessCambio;

	int min;

	//inicializador de variables de las nuevas poblaciones y sus respectivos fitness
	int** cromosomaNuevo = (int**)malloc(m * sizeof(int*));
	for (size_t a = 0; a < m; a++)
	{
		cromosomaNuevo[a] = (int*)malloc(quorum * sizeof(int));
	}
	double* fitnessPobNuevo = (double*)malloc(m * sizeof(double));

	double fitnessAnt;


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
	itera = itera + to_string(it+1) + ",";
	fitnessEvol = fitnessEvol + to_string(fitnessPob[0]) + ",";
	//incio de algoritmo
	fitnessAnt = fitnessPob[0];
	//cout << "FitnessPob0" << "   " << "FitnessAnt" << endl;
	while (k < max_k)
	{
		//guardamos fitness e iteraciones
		it++;
		//se seleciona cromosomas a cruzar
		cual1 = smallest_greater(cump, m, (double)uni2(mt));
		cual2 = smallest_greater(cump, m, (double)uni2(mt));
		//estos no pueden ser iguales
		while (cual1 == cual2)cual2 = smallest_greater(cump, m, (double)uni2(mt));

		//se guardan los cromosomas a cruzar
		int* cromosoma1 = (int*)malloc(quorum * sizeof(int));
		int* cromosoma2 = (int*)malloc(quorum * sizeof(int));
		memcpy(cromosoma1, cromosoma[cual1], quorum * sizeof(int));
		memcpy(cromosoma2, cromosoma[cual2], quorum * sizeof(int));

		//extraccion de los genes a cruzar
		mDis12 = notin(cromosoma1, cromosoma2, quorum, quorum);
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
			crossover12[a] = disimilar12[selecCrossover12[a]];
			crossover21[a] = disimilar21[selecCrossover21[a]];
		}

		//ordenamos los arreglos de los genes a cruzar
		//sort_bubble(crossover12, crossovern[0]);
		sort(crossover12, crossover12 + crossovern[0], &array_sort);
		//sort_bubble(crossover21, crossovern[0]);
		sort(crossover21, crossover21 + crossovern[0], &array_sort);
		//revisamos si los genes seleccionados se encuentran en el cromosoma aislado
		aBoolean1 = in_boolean(cromosoma1, crossover12, quorum, crossovern[0]);

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
		//sort_bubble(cromosoma1, quorum);
		sort(cromosoma1, cromosoma1 + quorum, &array_sort);
		//sort_bubble(cromosoma2, quorum);
		sort(cromosoma2, cromosoma2 + quorum, &array_sort);

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
		pmutacion = (double)uni2(mt);
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
			//sort_bubble(cromosoma1, quorum);
			sort(cromosoma1, cromosoma1 + quorum, &array_sort);
			//liberacion de memoria
			free(ar);
			free(cualSacar);
			free(mNotInCromosoma1[0]);
			free(mNotInCromosoma1[1]);
			free(mNotInCromosoma1);
			free(cualIntroducir);

		}
		//creamos la probabilidad de mutacion
		pmutacion = (double)uni2(mt);
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
			//sort_bubble(cromosoma2, quorum);
			sort(cromosoma2, cromosoma2 + quorum, &array_sort);
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
					//sort_bubble(cromosoma1, quorum);
					sort(cromosoma1, cromosoma1 + quorum, &array_sort);

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
					//sort_bubble(cromosoma2, quorum);
					sort(cromosoma2, cromosoma2 + quorum, &array_sort);

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
			fitnessPobNuevo[contCromNuevo] = eval_sol(cromosoma2, matDis, quorum);
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
			double* pNuevo = (double*)malloc(m * sizeof(double));
			double* cumpNuevo = (double*)malloc(m * sizeof(double));
			//si despues de terminar el while anterior la bandera flag3 es false
			if (flag3 == false)
			{
				//cambiamos el peor cromosoma de la poblacion actual por el mejor cromosoma de la poblacion nueva
				peor = m - 1;
				memcpy(cromosomaNuevo[peor], cromosoma[0], quorum * sizeof(int));
				fitnessPobNuevo[peor] = fitnessPob[0];
				order(fitnessPobNuevo, cromosomaNuevo, quorum, m);
				memcpy(pNuevo, p, m * sizeof(double));
				memcpy(cumpNuevo, cump, m * sizeof(double));

			}
			else
			{
				//de lo contrario mantenemos la probabilidad y la probabilidad acumulada
				memcpy(pNuevo, p, m * sizeof(double));
				memcpy(cumpNuevo, cump, m * sizeof(double));
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
			//int* cromosomaCambio = (int*)malloc(quorum * sizeof(int));
			//for (size_t j = m / 2; j < m; j++)
			//{
			//	sample(cromosomaCambio, n, quorum);
			//	sort_bubble(cromosomaCambio, quorum);
			//	fitnessCambio = eval_sol(cromosomaCambio, matDis, quorum);
			//	memcpy(cromosoma[j], cromosomaCambio, quorum * sizeof(int));
			//	fitnessPob[j] = fitnessCambio;
			//}
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
			//si el fitness del mejor cromosoma es igual al fitness del mejor cromosoma anterior guardamos el numero de iteraciones y el fitness
			if (fitnessPob[0] < fitnessAnt) {
				itera = itera + to_string(it) + ",";
				fitnessEvol = fitnessEvol + to_string(fitnessPob[0]) + ",";
				coalicionEvol = coalicionEvol + "[";
				for (size_t i = 0; i < quorum; i++)
				{
					coalicionEvol = coalicionEvol + to_string(cromosoma[0][i]) + ",";
				}
				replace(coalicionEvol.end() - 1, coalicionEvol.end(), ',', ']');
				coalicionEvol = coalicionEvol + ",";
				k = 0;
				//cout << fitnessPob[0] << "   " << fitnessAnt << endl;
				//guardamos el fitness del mejor cromosoma
				fitnessAnt = fitnessPob[0];
			}
			//liberamos memoria
			free(pNuevo);
			free(cumpNuevo);
			//free(cromosomaCambio);
			
		}
		//liberamos memoria
		free(cromosoma1);
		free(cromosoma2);
		free(crossovern);
		k = k + 1;
	}
	//cerramos el arreglo dentro del archivo json
	replace(itera.end() - 1, itera.end(), ',', ']');
	replace(fitnessEvol.end() - 1, fitnessEvol.end(), ',', ']');
	replace(coalicionEvol.end() - 1, coalicionEvol.end(), ',', ']');
	//calculo de tiempo
	auto tFinal = chrono::high_resolution_clock::now();
	double tTomado = chrono::duration_cast<chrono::nanoseconds>(tFinal - tInicial).count();
	tTomado *= 1e-9;

	//guardamos la informacion recopilada en un json
	resultados << "{\n\"m\": " << m << ",\n";
	resultados << "\"pmutacion_threshold\": " << pmutacion_threshold << ",\n";
	resultados << "\"pr\": " << pr << ",\n";
	resultados << "\"seed\": " << seed << ",\n";
	resultados << "\"numero_de_iteraciones\": " << it << ",\n";
	resultados << "\"fitness\": " << fixed<<fitnessPob[0]<<setprecision(9) << ",\n";
	resultados << "\"tiempo\": " << fixed << tTomado << setprecision(9) << ",\n";
	resultados << "\"coalicion\": [";

	for (size_t j = 0; j < quorum; j++)
	{
		//para el parlamento Chileno
		//if(j<quorum-1)resultados << data["diputados"][cromosoma[0][j]]["ID"] << ",";
		//else resultados << data["diputados"][cromosoma[0][j]]["ID"];

		//para el parlamento de estados unidos
		if (j < quorum - 1)resultados << cromosoma[0][j] << ",";
		else resultados << cromosoma[0][j];
	}
	resultados << "]\n" << "}";
	resultados.close();
	
	//guardamos datos para el histograma
	histo << "{\n\"iteraciones\": " << itera << ",\n";
	histo << "\"fitness\": " << fitnessEvol << ",\n";
	histo << "\"coaliciones\": " << coalicionEvol << "\n}";
	
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
	return EXIT_SUCCESS;
}