// Calling libraries and header files
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

using namespace std;
using json = nlohmann::json;

// Initial parameters
int sample_size = 38;
double mutation_threshold = 0.170002;
double selection_threshold = 0.141;

// Main function
int main(int argc, char *argv[])
{
	// Call the Json file
	ifstream file("votes.json");
	json data = json::parse(file);

	// Create and initialize the json files for the outputs
	ofstream results;
	results.open("results.json");
	ofstream histogram;
	histogram.open("hist.json");
	string evolution_of_coalition = "[\n";
	string iteration_string = "[";
	string evolution_of_fitness = "[";

	// Obtain the number of congressmen
	int n = data["rollcalls"][0]["votes"].size();

	// Initialize the distance matrix
	double **distance_matrix = (double **)malloc(n * sizeof(double *));
	for (size_t i = 0; i < n; i++)
	{
		distance_matrix[i] = (double *)malloc(n * sizeof(double));
	}

	// Fill the distance matrix
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			distance_matrix[i][j] = euclidian_distance(data["rollcalls"][0]["votes"][i]["x"], data["rollcalls"][0]["votes"][i]["y"], data["rollcalls"][0]["votes"][j]["x"], data["rollcalls"][0]["votes"][j]["y"]);
		}
	}

	// Time variable initialization for execution calculation
	auto initial_time = chrono::high_resolution_clock::now();

	// Quorum variable initialization
	int quorum = trunc(n / 2) + 1;

	// Check the parameters
	if (argc > 1)
	{
		sample_size = stoi(argv[1]);
		mutation_threshold = stod(argv[2]);
		selection_threshold = stod(argv[3]);
		seed = stoi(argv[4]);
	}
	// Initialize the random seed
	mt.seed(seed);

	// Random number generator between 0 and 1
	uniform_2 = uniform_real_distribution<double>(0, 1);

	// Initialize the vector of the solutions
	vector<Solution_structure> solutions;

	// Chromosome initialization
	for (size_t i = 0; i < sample_size; i++)
	{
		solutions.push_back(Solution_structure());
		solutions[i].coalition = (int *)malloc(quorum * sizeof(int));
	}
	// Fill the vector of the solutions with random solutions and evaluate them
	for (size_t i = 0; i < sample_size; i++)
	{
		sample(solutions[i].coalition, n, quorum);
		sort(solutions[i].coalition, solutions[i].coalition + quorum, &array_sort);
		solutions[i].fitness = evaluate_solution(solutions[i].coalition, distance_matrix, quorum);
	}

	// Sort the vector of the solutions
	sort(solutions.begin(), solutions.end(), &vector_initial_solutions_sort);

	// Save the evolution of the coalition for the output file
	evolution_of_coalition = evolution_of_coalition + "[";
	for (size_t i = 0; i < quorum; i++)
	{
		evolution_of_coalition = evolution_of_coalition + to_string(solutions[0].coalition[i]) + ",";
	}
	replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');
	evolution_of_coalition = evolution_of_coalition + ",";

	// Initialize the probability variables
	double *probability_of_selection = (double *)malloc(sample_size * sizeof(double));
	double *cumulative_probability = (double *)malloc(sample_size * sizeof(double));
	probability_of_selection[0] = 0.1;
	cumulative_probability[0] = 0.1;

	// Calculate the probability of selection and the cumulative probability
	for (size_t i = 1; i < sample_size; i++)
	{
		probability_of_selection[i] = selection_threshold * pow(1 - selection_threshold, i);
		cumulative_probability[i] = summation(probability_of_selection, i);
	}
	cumulative_probability[sample_size - 1] = 1;

	// Initialize the general variables
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

	// Initialize the variables of the new populations and their respective fitness
	int **new_chromosome = (int **)malloc(sample_size * sizeof(int *));
	for (size_t a = 0; a < sample_size; a++)
	{
		new_chromosome[a] = (int *)malloc(quorum * sizeof(int));
	}
	double *new_fitness_value = (double *)malloc(sample_size * sizeof(double));

	// Extra pointers
	bool *array_of_boolean_1 = nullptr;
	bool *array_of_boolean_2 = nullptr;
	int **distance_matrix_12 = nullptr;
	int **distance_matrix_21 = nullptr;
	int *dissimilar_12 = nullptr;
	int *dissimilar_21 = nullptr;
	int *minimal_matrix = nullptr;
	int *crossover = nullptr;
	int *selection_of_crossover_12 = nullptr;
	int *selection_of_crossover_21 = nullptr;
	int *crossover_12 = nullptr;
	int *crossover_21 = nullptr;
	int **boolean_positions_12 = nullptr;
	int **boolean_positions_21 = nullptr;
	int *chromosome_1 = nullptr;
	int *chromosome_2 = nullptr;

	iteration_string = iteration_string + to_string(iteration + 1) + ",";
	evolution_of_fitness = evolution_of_fitness + to_string(solutions[0].fitness) + ",";
	// Save the evolution of the coalition
	previous_fitness = solutions[0].fitness;
	// Start the algorithm
	while (k < maximum_k_value)
	{
		// Increase the iteration counter for the output file
		iteration++;

		// Selection of the chromosomes to crossover
		selection_1 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));
		selection_2 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));

		// They can't be the same
		while (selection_1 == selection_2)
			selection_2 = smallest_greater(cumulative_probability, sample_size, (double)uniform_2(mt));

		// Save the chromosomes previously selected
		int *chromosome_1 = (int *)malloc(quorum * sizeof(int));
		int *chromosome_2 = (int *)malloc(quorum * sizeof(int));
		memcpy(chromosome_1, solutions[selection_1].coalition, quorum * sizeof(int));
		memcpy(chromosome_2, solutions[selection_2].coalition, quorum * sizeof(int));

		// Extraction of the genes to crossover
		distance_matrix_12 = not_in(chromosome_1, chromosome_2, quorum, quorum);
		distance_matrix_21 = not_in(chromosome_2, chromosome_1, quorum, quorum);

		int *dissimilar_12 = (int *)malloc(distance_matrix_12[1][0] * sizeof(int));
		int *dissimilar_21 = (int *)malloc(distance_matrix_21[1][0] * sizeof(int));

		memcpy(dissimilar_12, distance_matrix_12[0], distance_matrix_12[1][0] * sizeof(int));
		memcpy(dissimilar_21, distance_matrix_21[0], distance_matrix_21[1][0] * sizeof(int));

		// Save and selection the amount of genes to crossover
		int *minimal_matrix = (int *)malloc(2 * sizeof(int));
		minimal_matrix[0] = distance_matrix_12[1][0];
		minimal_matrix[1] = distance_matrix_21[1][0];
		int *crossover = (int *)malloc(sizeof(int));
		minimum_value = minimum_of_array(minimal_matrix, 2);
		sample(crossover, minimum_value, 1);

		// Create and fill the arrays of selection of genes to crossover
		int *selection_of_crossover_12 = (int *)malloc(crossover[0] * sizeof(int));
		int *selection_of_crossover_21 = (int *)malloc(crossover[0] * sizeof(int));
		sample(selection_of_crossover_12, distance_matrix_12[1][0], crossover[0]);
		sample(selection_of_crossover_21, distance_matrix_21[1][0], crossover[0]);

		// Save the genes to crossover
		int *crossover_12 = (int *)malloc(crossover[0] * sizeof(int));
		int *crossover_21 = (int *)malloc(crossover[0] * sizeof(int));
		for (size_t a = 0; a < crossover[0]; a++)
		{
			crossover_12[a] = dissimilar_12[selection_of_crossover_12[a]];
			crossover_21[a] = dissimilar_21[selection_of_crossover_21[a]];
		}

		// Sort the arrays of the genes to crossover
		sort(crossover_12, crossover_12 + crossover[0], &array_sort);
		sort(crossover_21, crossover_21 + crossover[0], &array_sort);

		// Check whether the selected genes are located on the isolated chromosome
		array_of_boolean_1 = in_boolean(chromosome_1, crossover_12, quorum, crossover[0]);
		array_of_boolean_2 = in_boolean(chromosome_2, crossover_21, quorum, crossover[0]);

		int **boolean_positions_12 = which(array_of_boolean_1, quorum);
		int **boolean_positions_21 = which(array_of_boolean_2, quorum);

		free(array_of_boolean_1);
		free(array_of_boolean_2);

		// Clean the pointers
		int *gen_selection_12 = nullptr;
		int *gen_selection_21 = nullptr;
		// Based on the position of genes taken previously, now if we carry out the crossings to the isolated chromosomes
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
		// Sort the chromosomes
		sort(chromosome_1, chromosome_1 + quorum, &array_sort);
		sort(chromosome_2, chromosome_2 + quorum, &array_sort);

		// Free the memory
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

		// Create the probability of mutation
		probability_of_mutation = (double)uniform_2(mt);

		// Check if the chromosome_1 mutates
		if (probability_of_mutation < mutation_threshold)
		{
			// Selection of genes to mutate
			int *which_get = (int *)malloc(sizeof(int));
			sample(which_get, quorum, 1);
			int **matrix_not_in_chromosome_1 = nullptr;
			int *base_array = create_array(n);
			matrix_not_in_chromosome_1 = not_in(base_array, chromosome_1, n, quorum);
			int *not_in_chromosome_1 = nullptr;
			not_in_chromosome_1 = matrix_not_in_chromosome_1[0];
			int *which_insert = (int *)malloc(sizeof(int));
			sample_array(which_insert, 1, not_in_chromosome_1, matrix_not_in_chromosome_1[1][0]);

			// Realize the mutation and sort
			chromosome_1[which_get[0]] = which_insert[0];
			sort(chromosome_1, chromosome_1 + quorum, &array_sort);

			// Free the memory
			free(base_array);
			free(which_get);
			free(matrix_not_in_chromosome_1[0]);
			free(matrix_not_in_chromosome_1[1]);
			free(matrix_not_in_chromosome_1);
			free(which_insert);
		}
		// Create the probability of mutation
		probability_of_mutation = (double)uniform_2(mt);

		// Check if the chromosome_2 mutates
		if (probability_of_mutation < mutation_threshold)
		{
			// Selection of genes to mutate
			int *which_get = (int *)malloc(sizeof(int));
			sample(which_get, quorum, 1);
			int **matrix_not_in_chromosome_2 = nullptr;
			int *base_array = create_array(n);
			matrix_not_in_chromosome_2 = not_in(base_array, chromosome_2, n, quorum);
			int *not_in_chromosome_2 = nullptr;
			not_in_chromosome_2 = matrix_not_in_chromosome_2[0];
			int *which_insert = (int *)malloc(sizeof(int));
			sample_array(which_insert, 1, not_in_chromosome_2, matrix_not_in_chromosome_2[1][0]);

			// Realize the mutation and sort
			chromosome_2[which_get[0]] = which_insert[0];
			sort(chromosome_2, chromosome_2 + quorum, &array_sort);
			// Free the memory
			free(base_array);
			free(which_get);
			free(matrix_not_in_chromosome_2[0]);
			free(matrix_not_in_chromosome_2[1]);
			free(matrix_not_in_chromosome_2);
			free(which_insert);
		}

		// Check if it is the first iteration
		if (i == 0)
		{
			// Initialize the new population with the isolated chromosomes and recalculate their fitness
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_1, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_1, distance_matrix, quorum);
			counter_of_new_chromosomes++;
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_2, quorum * sizeof(int));

			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_2, distance_matrix, quorum);
			counter_of_new_chromosomes++;
		}
		// If not the first iteration
		else
		{
			// Initialize the flag and counter
			flag_1 = true;
			index_to_pop = 0;
			while (flag_1)
			{
				// Clear the pointer
				array_of_boolean_1 = nullptr;
				// Save an array of booleans if it finds genes from chromosome_1 in the array of chromosomes in the position index_to_pop
				array_of_boolean_1 = in_boolean(chromosome_1, new_chromosome[index_to_pop], quorum, quorum);
				// If the sum of the booleans is equal to the quorum
				if (summation_of_booleans(array_of_boolean_1, quorum) == quorum)
				{
					int *which_get = (int *)malloc(sizeof(int));

					// Selection of random gen to mutate
					sample(which_get, quorum, 1);
					int **matrix_not_in_chromosome_1 = nullptr;

					// Create an array
					int *base_array = create_array(n);

					// Select the elements that are not in chromosome_1
					matrix_not_in_chromosome_1 = not_in(base_array, chromosome_1, n, quorum);
					int *not_in_chromosome_1 = nullptr;
					not_in_chromosome_1 = matrix_not_in_chromosome_1[0];
					int *which_insert = (int *)malloc(sizeof(int));

					// Select an array of length 1 of elements that are not in chromosome_1 randomly
					sample_array(which_insert, 1, not_in_chromosome_1, matrix_not_in_chromosome_1[1][0]);

					// Realize the mutation and sort
					chromosome_1[which_get[0]] = which_insert[0];
					sort(chromosome_1, chromosome_1 + quorum, &array_sort);

					// Clear the memory
					free(base_array);
					free(which_get);
					free(matrix_not_in_chromosome_1[0]);
					free(matrix_not_in_chromosome_1[1]);
					free(matrix_not_in_chromosome_1);
					free(which_insert);
					// Reset the counter
					index_to_pop = 0;
				}
				else
				{
					// If we have already traversed the entire new population
					if (index_to_pop >= counter_of_new_chromosomes - 1)
					{
						// Get out of the loop
						flag_1 = false;
					}
					else
					{
						// Otherwise we increment the counter
						index_to_pop++;
					}
				}
				// Free the memory
				free(array_of_boolean_1);
			}
			// The same explained previously but for chromosome_2
			flag_1 = true;
			index_to_pop = 0;
			while (flag_1)
			{
				array_of_boolean_2 = nullptr;
				array_of_boolean_2 = in_boolean(chromosome_2, new_chromosome[index_to_pop], quorum, quorum);
				if (summation_of_booleans(array_of_boolean_2, quorum) == quorum)
				{
					int *which_get = (int *)malloc(sizeof(int));
					sample(which_get, quorum, 1);
					int **matrix_not_in_chromosome_2 = nullptr;
					int *base_array = create_array(n);
					matrix_not_in_chromosome_2 = not_in(base_array, chromosome_2, n, quorum);
					int *not_in_chromosome_2 = nullptr;
					not_in_chromosome_2 = matrix_not_in_chromosome_2[0];
					int *which_insert = (int *)malloc(sizeof(int));
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

			// save the changes in the array of new chromosomes
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_1, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_1, distance_matrix, quorum);
			counter_of_new_chromosomes++;
			memcpy(new_chromosome[counter_of_new_chromosomes], chromosome_2, quorum * sizeof(int));
			new_fitness_value[counter_of_new_chromosomes] = evaluate_solution(chromosome_2, distance_matrix, quorum);
			counter_of_new_chromosomes++;
		}

		// increase the counter of iterations
		i++;
		// If we have reached the middle of the population
		if (i == sample_size / 2)
		{
			// reset the counter
			i = 0;
			counter_of_new_chromosomes = 0;

			// sort the new population
			order(new_fitness_value, new_chromosome, quorum, sample_size);

			// Create a flags
			flag_1 = true;
			flag_2 = false;
			index_to_pop = 0;

			while (flag_1)
			{
				array_of_boolean_1 = nullptr;
				// Save an array of booleans if it finds genes from chromosome_1 in the array of chromosomes at position index_to_pop
				array_of_boolean_1 = in_boolean(solutions[0].coalition, new_chromosome[index_to_pop], quorum, quorum);

				// If the sum of these is equal to the quorum
				if (summation_of_booleans(array_of_boolean_1, quorum) == quorum)
				{
					// We change the flags
					flag_1 = false;
					flag_2 = true;
				}
				else
				{
					// Otherwise we increment the counter
					index_to_pop++;
				}
				// If the counter indexpop reaches the size of the population
				if (index_to_pop > sample_size - 1)
				{
					// We stop the cycle
					flag_1 = false;
				}
				// Free the memory
				free(array_of_boolean_1);
			}
			double *new_probability_of_selection = (double *)malloc(sample_size * sizeof(double));
			double *new_cumulative_probability = (double *)malloc(sample_size * sizeof(double));
			// If after finishing the previous while the flag flag_2 is false
			if (flag_2 == false)
			{
				// Change the worst chromosome of the current population by the best chromosome of the new population
				worst_chromosome = sample_size - 1;
				memcpy(new_chromosome[worst_chromosome], solutions[0].coalition, quorum * sizeof(int));
				new_fitness_value[worst_chromosome] = solutions[0].fitness;
				order(new_fitness_value, new_chromosome, quorum, sample_size);
				memcpy(new_probability_of_selection, probability_of_selection, sample_size * sizeof(double));
				memcpy(new_cumulative_probability, cumulative_probability, sample_size * sizeof(double));
			}
			else
			{
				// Otherwise we keep the probability and the cumulative probability
				memcpy(new_probability_of_selection, probability_of_selection, sample_size * sizeof(double));
				memcpy(new_cumulative_probability, cumulative_probability, sample_size * sizeof(double));
			}
			// Change the current population by the new population
			for (size_t a = 0; a < sample_size; a++)
			{
				memcpy(solutions[a].coalition, new_chromosome[a], quorum * sizeof(int));
				solutions[a].fitness = new_fitness_value[a];
				probability_of_selection[a] = new_probability_of_selection[a];
				cumulative_probability[a] = new_cumulative_probability[a];
			}

			// Sort the population
			sort(solutions.begin(), solutions.end(), &vector_initial_solutions_sort);

			// Initialize the probability and the cumulative probability
			probability_of_selection[0] = selection_threshold;
			cumulative_probability[0] = selection_threshold;
			// We finish filling the probability and the cumulative probability
			for (size_t j = 1; j < sample_size; j++)
			{
				probability_of_selection[j] = selection_threshold * pow(1 - selection_threshold, j);
				cumulative_probability[j] = summation(probability_of_selection, j);
			}

			// We leave the last accumulated probability in 1
			cumulative_probability[sample_size - 1] = 1;

			// reset the counter of iterations
			i = 0;
			// If the fitness of the best chromosome is equal to the fitness of the previous best chromosome we save the number of iterations and the fitness
			if (solutions[0].fitness < previous_fitness)
			{
				// Save this solution in the output file
				iteration_string = iteration_string + to_string(iteration) + ",";
				evolution_of_fitness = evolution_of_fitness + to_string(solutions[0].fitness) + ",";
				evolution_of_coalition = evolution_of_coalition + "[";
				for (size_t l = 0; l < quorum; l++)
				{
					evolution_of_coalition = evolution_of_coalition + to_string(solutions[0].coalition[l]) + ",";
				}
				replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');
				evolution_of_coalition = evolution_of_coalition + ",";
				// Reset the counter of iterations
				k = 0;

				// Save the fitness of the best chromosome
				previous_fitness = solutions[0].fitness;
			}
			// Free the memory
			free(new_probability_of_selection);
			free(new_cumulative_probability);
		}
		// Free the memory
		free(chromosome_1);
		free(chromosome_2);
		free(crossover);
		// Increment the counter of iterations
		k = k + 1;
	}
	// Close the array inside the json file
	replace(iteration_string.end() - 1, iteration_string.end(), ',', ']');
	replace(evolution_of_fitness.end() - 1, evolution_of_fitness.end(), ',', ']');
	replace(evolution_of_coalition.end() - 1, evolution_of_coalition.end(), ',', ']');

	// Stop the clock
	auto final_time = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(final_time - initial_time).count();
	// Convert the time taken by the algorithm to seconds
	time_taken *= 1e-9;

	// Save the information collected in a json
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
		if (j < quorum - 1)
			results << solutions[0].coalition[j] << ",";
		else
			results << solutions[0].coalition[j];
	}
	results << "]\n"
			<< "}";
	results.close();

	// Save the information collected in a histogram json
	histogram << "{\n\"iterations\": " << iteration_string << ",\n";
	histogram << "\"fitness\": " << evolution_of_fitness << ",\n";
	histogram << "\"coalition\": " << evolution_of_coalition << "\n}";

	// Free the memory
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

	// Finish the program
	return EXIT_SUCCESS;
}