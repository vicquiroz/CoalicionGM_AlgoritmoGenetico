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
int sample_size = 38;
double mutation_threshold = 0.170002;
double selection_threshold = 0.141;

//funcion principal
int main(int argc, char* argv[])
{
	//llamada de JSON
	//para parlamento de estados unidos
	ifstream file("votes.json");
	json data = json::parse(file);


	//se crea y abre el file de salida
	ofstream results;
	results.open("results.json");

	ofstream histogram;
	histogram.open("hist.json");
	string evolution_of_coalition = "[\n";
	string iteration_string = "[";
	string evolution_of_fitness = "[";
	
	//de los JSON se obtiene la cantidad de diputados
	//para parlamento de estados unidos
	int n = data["rollcalls"][0]["votes"].size();

	//creacion de la matriz de distancia
	double** distance_matrix = (double**)malloc(n * sizeof(double*));
	for (size_t i = 0; i < n; i++)
	{
		distance_matrix[i] = (double*)malloc(n * sizeof(double));
	}

	//rellenado de la matriz de distancia
	// Matriz rellenando por completo
	for (size_t i = 0; i <n; i++)
	{

		//cout << i << endl;
		for (size_t j = 0; j <n; j++)
		{	
			distance_matrix[i][j] = euclidian_distance(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]);
		}
	}

	//inicializacion de tiempo para calculo de ejecucion
	auto initial_time = chrono::high_resolution_clock::now();

	//inicializacion de quorum
	//para parlamento de estados unidos
	int quorum = trunc(n / 2) + 1;

	//revision de parametros de entrada
	if (argc > 1)
	{
		sample_size = stoi(argv[1]);
		mutation_threshold = stod(argv[2]);
		selection_threshold = stod(argv[3]);
		seed = stoi(argv[4]);
	}
	//inicializacion de semilla
	mt.seed(seed);

	//generador de double's random entre 0 y 1
	uniform_2 = uniform_real_distribution<double>(0, 1);

	vector<Solution_structure> solutions;

	//inicializacion de cromosomas
	for (size_t i = 0; i < sample_size; i++)
	{
		solutions.push_back(Solution_structure());
		solutions[i].coalition = (int*)malloc(quorum * sizeof(int));
		//cromosoma[i] = (int*)malloc(quorum * sizeof(int));
	}

	for (size_t i = 0; i < sample_size; i++)
	{
		sample(solutions[i].coalition, n, quorum);
		sort(solutions[i].coalition, solutions[i].coalition + quorum, &array_sort);
		solutions[i].fitness = evaluate_solution(solutions[i].coalition, distance_matrix, quorum);
	}
	
	//ordenamiento de fitness y cromosomas
	sort(solutions.begin(), solutions.end(), &vector_initial_solutions_sort);
	evolution_of_coalition = evolution_of_coalition + "[";
	for (size_t i = 0; i < quorum; i++)
	{
		evolution_of_coalition = evolution_of_coalition + to_string(solutions[0].coalition[i]) + ",";
	}
	replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');
	evolution_of_coalition = evolution_of_coalition + ",";

	//inicializacion de variables de probabilidad
	double* probability_of_selection = (double*)malloc(sample_size * sizeof(double));
	double* cumulative_probability = (double*)malloc(sample_size * sizeof(double));

	probability_of_selection[0] = 0.1;
	cumulative_probability[0] = 0.1;

	//calculo de probabilidad
	for (size_t i = 1; i < sample_size; i++)
	{
		probability_of_selection[i] = selection_threshold * pow(1 - selection_threshold, i);
		cumulative_probability[i] = summation(probability_of_selection, i);
	}
	cumulative_probability[sample_size - 1] = 1;
	
	//inicializador de variables
	int maximum_k_value = 200 * trunc((n + quorum) / sample_size);
	int i = 0;
	int k = 0;
	int iteration = 0;
	int selection_1;
	int selection_2;
	double probability_of_mutation;
	bool flag_1;
	bool flag_2;
	int index_to_pop;
	int counter_of_new_chromosomes = 0;
	int worst_chromosome;
	int minimum_value;
	double previous_fitness;
	
	//inicializador de variables de las nuevas poblaciones y sus respectivos fitness
	int** new_chromosome = (int**)malloc(sample_size * sizeof(int*));
	for (size_t a = 0; a < sample_size; a++)
	{
		new_chromosome[a] = (int*)malloc(quorum * sizeof(int));
	}
	double* new_fitness_value = (double*)malloc(sample_size * sizeof(double));

	// Reservas extras 

	bool* array_of_boolean_1 = nullptr;
	bool* array_of_boolean_2 = nullptr;
	int** distance_matrix_12 = nullptr;
	int** distance_matrix_21 = nullptr;
	int* dissimilar_12 = nullptr;
	int* dissimilar_21 = nullptr;
	int* minimal_matrix = nullptr;
	int* crossover = nullptr;
	int* selection_of_crossover_12 = nullptr;
	int* selection_of_crossover_21 = nullptr;
	int* crossover_12 = nullptr;
	int* crossover_21 = nullptr;
	int** boolean_positions_12 = nullptr;
	int** boolean_positions_21 = nullptr;
	int* chromosome_1 = nullptr;
	int* chromosome_2 = nullptr;

	iteration_string = iteration_string + to_string(iteration+1) + ",";
	evolution_of_fitness = evolution_of_fitness + to_string(solutions[0].fitness) + ",";
	//incio de algoritmo
	previous_fitness = solutions[0].fitness;
	while (k < maximum_k_value)
	{
		//guardamos fitness e iteraciones
		iteration++;
		//se seleciona cromosomas a cruzar
		selection_1 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));
		selection_2 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));
		//estos no pueden ser iguales
		while (selection_1 == selection_2) selection_2 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));

		//se guardan los cromosomas a cruzar
		int* chromosome_1 = (int*)malloc(quorum * sizeof(int));
		int* chromosome_2 = (int*)malloc(quorum * sizeof(int));
		//memcpy(chromosome_1, cromosoma[selection_1], quorum * sizeof(int));
		memcpy(chromosome_1, solutions[selection_1].coalition, quorum * sizeof(int));
		//memcpy(chromosome_2, cromosoma[selection_2], quorum * sizeof(int));
		memcpy(chromosome_2, solutions[selection_2].coalition, quorum * sizeof(int));

		//extraccion de los genes a cruzar
		distance_matrix_12 = not_in(chromosome_1, chromosome_2, quorum, quorum);
		distance_matrix_21 = not_in(chromosome_2, chromosome_1, quorum, quorum);

		int* dissimilar_12 = (int*)malloc(distance_matrix_12[1][0] * sizeof(int));
		int* dissimilar_21 = (int*)malloc(distance_matrix_21[1][0] * sizeof(int));

		memcpy(dissimilar_12, distance_matrix_12[0], distance_matrix_12[1][0] * sizeof(int));
		memcpy(dissimilar_21, distance_matrix_21[0], distance_matrix_21[1][0] * sizeof(int));

		//guardamos la cantidad de genes a cruzar
		int* minimal_matrix = (int*)malloc(2 * sizeof(int));
		minimal_matrix[0] = distance_matrix_12[1][0];
		minimal_matrix[1] = distance_matrix_21[1][0];

		//seleccion de cantidad de genes a cruzar
		int* crossover = (int*)malloc(sizeof(int));

		minimum_value = minimum_of_array(minimal_matrix, 2);
		sample(crossover, minimum_value, 1);

		//creamos y rellenamos los arrays de seleccion de genes a cruzar
		int* selection_of_crossover_12 = (int*)malloc(crossover[0] * sizeof(int));
		int* selection_of_crossover_21 = (int*)malloc(crossover[0] * sizeof(int));

		sample(selection_of_crossover_12, distance_matrix_12[1][0], crossover[0]);
		sample(selection_of_crossover_21, distance_matrix_21[1][0], crossover[0]);

		//guardamos los genes a cruzar
		int* crossover_12 = (int*)malloc(crossover[0] * sizeof(int));
		int* crossover_21 = (int*)malloc(crossover[0] * sizeof(int));

		for (size_t a = 0; a < crossover[0]; a++)
		{
			crossover_12[a] = dissimilar_12[selection_of_crossover_12[a]];
			crossover_21[a] = dissimilar_21[selection_of_crossover_21[a]];
		}

		//ordenamos los arreglos de los genes a cruzar
		sort(crossover_12, crossover_12 + crossover[0], &array_sort);
		sort(crossover_21, crossover_21 + crossover[0], &array_sort);
		//revisamos si los genes seleccionados se encuentran en el cromosoma aislado
		array_of_boolean_1 = in_boolean(chromosome_1, crossover_12, quorum, crossover[0]);

		array_of_boolean_2 = in_boolean(chromosome_2, crossover_21, quorum, crossover[0]);

		int** boolean_positions_12 = which(array_of_boolean_1, quorum);
		int** boolean_positions_21 = which(array_of_boolean_2, quorum);

		free(array_of_boolean_1);
		free(array_of_boolean_2);

		int* gen_selection_12 = nullptr;
		int* gen_selection_21 = nullptr;
		//en base a la posicion de genes sacada anteriormente ahora si realizamos los cruces a los cromosomas aislados
		gen_selection_12 = boolean_positions_12[0];
		gen_selection_21 = boolean_positions_21[0];
		for (size_t a = 0; a < boolean_positions_12[1][0]; a++)
		{
			chromosome_1[gen_selection_12[a]] = crossover_21[a];
		}
		for (size_t a = 0; a < boolean_positions_21[1][0]; a++)
		{
			chromosome_2[gen_selection_21[a]] = crossover_12[a];
		}
		//se ordenan
		sort(chromosome_1, chromosome_1 + quorum, &array_sort);
		sort(chromosome_2, chromosome_2 + quorum, &array_sort);

		//liberacion de memoria
		free(minimal_matrix);
		free(distance_matrix_12[0]);
		free(distance_matrix_12[1]);
		free(distance_matrix_12);
		free(distance_matrix_21[0]);
		free(distance_matrix_21[1]);
		free(distance_matrix_21);
		free(boolean_positions_12[0]);
		free(boolean_positions_12[1]);
		free(boolean_positions_12);
		free(boolean_positions_21[0]);
		free(boolean_positions_21[1]);
		free(boolean_positions_21);
		free(dissimilar_12);
		free(dissimilar_21);
		free(crossover_12);
		free(crossover_21);
		free(selection_of_crossover_12);
		free(selection_of_crossover_21);
		//creamos la probabilidad de mutacion
		probability_of_mutation = (double)uniform_2(mt);
		//comprobamos si se muta el chromosome_1
		if (probability_of_mutation < mutation_threshold) {
			//seleccion de genes a mutar
			int* which_get = (int*)malloc(sizeof(int));
			sample(which_get, quorum, 1);
			int** matrix_not_in_chromosome_1 = nullptr;
			int* base_array = create_array(n);
			matrix_not_in_chromosome_1 = not_in(base_array, chromosome_1, n, quorum);
			int* not_in_chromosome_1 = nullptr;
			not_in_chromosome_1 = matrix_not_in_chromosome_1[0];
			int* which_insert = (int*)malloc(sizeof(int));
			sample_array(which_insert, 1, not_in_chromosome_1, matrix_not_in_chromosome_1[1][0]);
			
			//se realiza la mutacion y se ordena
			chromosome_1[which_get[0]] = which_insert[0];
			sort(chromosome_1, chromosome_1 + quorum, &array_sort);
			
			//liberacion de memoria
			free(base_array);
			free(which_get);
			free(matrix_not_in_chromosome_1[0]);
			free(matrix_not_in_chromosome_1[1]);
			free(matrix_not_in_chromosome_1);
			free(which_insert);

		}
		//creamos la probabilidad de mutacion
		probability_of_mutation = (double)uniform_2(mt);
		
		//comprobamos si se muta el chromosome_2
		if (probability_of_mutation < mutation_threshold) {
			//seleccion de genes a mutar
			int* which_get = (int*)malloc(sizeof(int));
			sample(which_get, quorum, 1);
			int** matrix_not_in_chromosome_2 = nullptr;
			int* base_array = create_array(n);
			matrix_not_in_chromosome_2 = not_in(base_array, chromosome_2, n, quorum);
			int* not_in_chromosome_2 = nullptr;
			not_in_chromosome_2 = matrix_not_in_chromosome_2[0];
			int* which_insert = (int*)malloc(sizeof(int));
			sample_array(which_insert, 1, not_in_chromosome_2, matrix_not_in_chromosome_2[1][0]);
			//se realiza la mutacion y se ordena
			chromosome_2[which_get[0]] = which_insert[0];
			sort(chromosome_2, chromosome_2 + quorum, &array_sort);
			//liberacion de memoria
			free(base_array);
			free(which_get);
			free(matrix_not_in_chromosome_2[0]);
			free(matrix_not_in_chromosome_2[1]);
			free(matrix_not_in_chromosome_2);
			free(which_insert);

		}
		//comprobacion si es la primera iteracion
		if (i == 0) {
			//inicializamos la nueva poblacion con los cromosomas aislados y recalculamos su fitness
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_1, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_1, distance_matrix, quorum);
			counter_of_new_chromosomes++;
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_2, quorum * sizeof(int));

			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_2, distance_matrix, quorum);
			counter_of_new_chromosomes++;
		}
		//comprobacion si no es la primera iteracion
		else {
			//inicializador de bandera y contador
			flag_1 = true;
			index_to_pop = 0;
			while (flag_1) {
				array_of_boolean_1 = nullptr;
				//guardamos un arreglo de los booleanos si encuentra genes del chromosome_1 en el arreglo de cromosomas en la posición index_to_pop
				array_of_boolean_1 = in_boolean(chromosome_1, new_chromosome[index_to_pop], quorum, quorum);
				//si la suma de estos es igual al quorum
				if (summation_of_booleans(array_of_boolean_1, quorum) == quorum) {
					int* which_get = (int*)malloc(sizeof(int));
					// elegimos un gen aleatorio del para sacarlo
					sample(which_get, quorum, 1);
					int** matrix_not_in_chromosome_1 = nullptr;
					//creamos un arreglo
					int* base_array = create_array(n);
					//selecciona los elementos que no estan en chromosome_1
					matrix_not_in_chromosome_1 = not_in(base_array, chromosome_1, n, quorum);
					int* not_in_chromosome_1 = nullptr;
					not_in_chromosome_1 = matrix_not_in_chromosome_1[0];
					int* which_insert = (int*)malloc(sizeof(int));
					//selecciona un arreglo de largo 1 de elementos que no estan en chromosome_1 de manera aleatoria
					sample_array(which_insert, 1, not_in_chromosome_1, matrix_not_in_chromosome_1[1][0]);
					//se realiza la mutacion y se ordena
					chromosome_1[which_get[0]] = which_insert[0];
					//sort_bubble(chromosome_1, quorum);
					sort(chromosome_1, chromosome_1 + quorum, &array_sort);

					//limpiar memoria
					free(base_array);
					free(which_get);
					free(matrix_not_in_chromosome_1[0]);
					free(matrix_not_in_chromosome_1[1]);
					free(matrix_not_in_chromosome_1);
					free(which_insert);
					//resetear contador
					index_to_pop = 0;
				}
				else
				{
					//si ya recorrimos toda la poblacion nueva
					if (index_to_pop >= counter_of_new_chromosomes - 1)
					{
						//salimos del ciclo
						flag_1 = false;
					}
					else
					{
						//de lo contrario incrementamos el contador
						index_to_pop++;
					}
				}
				//liberacion de memoria
				free(array_of_boolean_1);
			}
			//lo mismo explicado anteriormente pero para el chromosome_2
			flag_1 = true;
			index_to_pop = 0;
			while (flag_1) {
				array_of_boolean_2 = nullptr;
				array_of_boolean_2 = in_boolean(chromosome_2, new_chromosome[index_to_pop], quorum, quorum);
				if (summation_of_booleans(array_of_boolean_2, quorum) == quorum) {
					int* which_get = (int*)malloc(sizeof(int));
					sample(which_get, quorum, 1);
					int** matrix_not_in_chromosome_2 = nullptr;
					int* base_array = create_array(n);
					matrix_not_in_chromosome_2 = not_in(base_array, chromosome_2, n, quorum);
					int* not_in_chromosome_2 = nullptr;
					not_in_chromosome_2 = matrix_not_in_chromosome_2[0];
					int* which_insert = (int*)malloc(sizeof(int));
					sample_array(which_insert, 1, not_in_chromosome_2, matrix_not_in_chromosome_2[1][0]);
					chromosome_2[which_get[0]] = which_insert[0];
					sort(chromosome_2, chromosome_2 + quorum, &array_sort);

					free(base_array);
					free(which_get);
					free(matrix_not_in_chromosome_2[0]);
					free(matrix_not_in_chromosome_2[1]);
					free(matrix_not_in_chromosome_2);
					free(which_insert);

					index_to_pop = 0;
				}
				else
				{
					if (index_to_pop == counter_of_new_chromosomes - 1)
					{
						flag_1 = false;
					}
					else
					{
						index_to_pop++;
					}
				}
				free(array_of_boolean_2);
			}

			//guardamos los cambios en el arreglo de cromosomas nuevos
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_1, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_1, distance_matrix, quorum);
			counter_of_new_chromosomes++;
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_2, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_2, distance_matrix, quorum);
			counter_of_new_chromosomes++;
		}
		//aumentamos contador de iteraciones
		i++;
		//si llegamos a la mitad de la poblacion
		if (i == sample_size / 2)
		{
			//reseteamos contador
			i = 0;
			counter_of_new_chromosomes = 0;
			//ordenamos la poblacion nueva
			order(new_fitness_value, new_chromosome, quorum, sample_size);
			
			//creacion de bandera
			flag_1 = true;
			flag_2 = false;
			index_to_pop = 0;

			while (flag_1)
			{
				array_of_boolean_1 = nullptr;
				//guardamos un arreglo de los booleanos si encuentra genes del chromosome_1 en el arreglo de cromosomas en la posición index_to_pop
				array_of_boolean_1 = in_boolean(solutions[0].coalition, new_chromosome[index_to_pop], quorum, quorum);
				//si la suma de estos es igual al quorum
				if (summation_of_booleans(array_of_boolean_1, quorum) == quorum)
				{
					//cambiamos las banderas
					flag_1 = false;
					flag_2 = true;
				}
				else
				{
					//de lo contrario incrementamos el contador
					index_to_pop++;
				}
				//si el contador indexpop llega al tamaño de la poplacion
				if (index_to_pop > sample_size - 1)
				{
					//detenemos el ciclo
					flag_1 = false;
				}
				//liberamos memoria
				free(array_of_boolean_1);
			}
			double* new_probability_of_selection = (double*)malloc(sample_size * sizeof(double));
			double* new_cumulative_probability = (double*)malloc(sample_size * sizeof(double));
			//si despues de terminar el while anterior la bandera flag_2 es false
			if (flag_2 == false)
			{
				//cambiamos el worst_chromosome cromosoma de la poblacion actual por el mejor cromosoma de la poblacion nueva
				worst_chromosome = sample_size - 1;
				memcpy(new_chromosome[worst_chromosome], solutions[0].coalition, quorum * sizeof(int));
				new_fitness_value[worst_chromosome] = solutions[0].fitness;
				order(new_fitness_value, new_chromosome, quorum, sample_size);
				memcpy(new_probability_of_selection, probability_of_selection, sample_size * sizeof(double));
				memcpy(new_cumulative_probability, cumulative_probability, sample_size * sizeof(double));
			}
			else
			{
				//de lo contrario mantenemos la probabilidad y la probabilidad acumulada
				memcpy(new_probability_of_selection, probability_of_selection, sample_size * sizeof(double));
				memcpy(new_cumulative_probability, cumulative_probability, sample_size * sizeof(double));
			}
			//cambiamos la poblacion actual por la poblacion nueva
			for (size_t a = 0; a < sample_size; a++)
			{
				memcpy(solutions[a].coalition, new_chromosome[a], quorum * sizeof(int));
				solutions[a].fitness = new_fitness_value[a];
				probability_of_selection[a] = new_probability_of_selection[a];
				cumulative_probability[a] = new_cumulative_probability[a];
			}
			//ordenamos la poblacion
			sort(solutions.begin(), solutions.end(), &vector_initial_solutions_sort);
			//inicializamos la probabilidad y la probabilidad acumulada
			probability_of_selection[0] = selection_threshold;
			cumulative_probability[0] = selection_threshold;
			//terminamos de rellenar la probabilidad y la probabilidad acumulada
			for (size_t j = 1; j < sample_size; j++)
			{
				probability_of_selection[j] = selection_threshold * pow(1 - selection_threshold, j);
				cumulative_probability[j] = summation(probability_of_selection, j);
			}
			//dejamos la ultima probabilidad acumulada en 1
			cumulative_probability[sample_size - 1] = 1;
			//reinicializamos el contador de iteraciones
			i = 0;
			//si el fitness del mejor cromosoma es igual al fitness del mejor cromosoma anterior guardamos el numero de iteraciones y el fitness
			if(solutions[0].fitness<previous_fitness){
				iteration_string = iteration_string + to_string(iteration) + ",";
				evolution_of_fitness = evolution_of_fitness + to_string(solutions[0].fitness) + ",";
				evolution_of_coalition = evolution_of_coalition + "[";
				for (size_t l = 0; l < quorum; l++)
				{
					evolution_of_coalition = evolution_of_coalition + to_string(solutions[0].coalition[l]) + ",";
				}
				replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');
				evolution_of_coalition = evolution_of_coalition + ",";
				k = 0;

				//guardamos el fitness del mejor cromosoma
				previous_fitness = solutions[0].fitness;
			}
			//liberamos memoria
			free(new_probability_of_selection);
			free(new_cumulative_probability);
			//free(cromosomaCambio);
			
		}
		//liberamos memoria
		free(chromosome_1);
		free(chromosome_2);
		free(crossover);
		k = k + 1;
	}
	//cerramos el arreglo dentro del file json
	replace(iteration_string.end() - 1, iteration_string.end(), ',', ']');
	replace(evolution_of_fitness.end() - 1, evolution_of_fitness.end(), ',', ']');
	replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');
	//calculo de tiempo
	auto final_time = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(final_time - initial_time).count();
	time_taken *= 1e-9;

	//guardamos la informacion recopilada en un json
	results << "{\n\"sample_size\": " << sample_size << ",\n";
	results << "\"mutation_threshold\": " << mutation_threshold << ",\n";
	results << "\"selection_threshold\": " << selection_threshold << ",\n";
	results << "\"seed\": " << seed << ",\n";
	results << "\"number_of_iteration\": " << iteration << ",\n";
	results << "\"fitness\": " << fixed << solutions[0].fitness << setprecision(9) << ",\n";
	results << "\"time_taken\": " << fixed << time_taken << setprecision(9) << ",\n";
	results << "\"coalition\": [";

	for (size_t j = 0; j < quorum; j++)
	{
		//para el parlamento de estados unidos
		if (j < quorum - 1)results << solutions[0].coalition[j] << ",";
		else results << solutions[0].coalition[j];
	}
	results << "]\n" << "}";
	results.close();
	
	//guardamos datos para el histograma
	histogram << "{\n\"iterations\": " << iteration_string << ",\n";
	histogram << "\"fitness\": " << evolution_of_fitness << ",\n";
	histogram << "\"coalition\": " << evolution_of_coalition << "\n}";
	
	//liberamos memoria
	free(new_fitness_value);
	free(probability_of_selection);
	free(cumulative_probability);
	for (size_t j = 0; j < sample_size; j++)
	{
		free(new_chromosome[j]);
	}
	for (size_t l = 0; l < n; l++)
	{
		free(distance_matrix[l]);
	}
	free(new_chromosome);
	free(distance_matrix);
	return EXIT_SUCCESS;
}