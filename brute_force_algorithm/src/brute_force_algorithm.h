// brute_force_algorithm.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos de un proyecto.
#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <math.h>
#include <iostream>
#include <float.h>
#include <nlohmann/json.hpp>
#include <tuple>
using namespace std;
bool vector_sort(int const& lvd, int const& rvd)
{
	return lvd < rvd;
}

// Function to calculate the distance between two points
double euclidian_distance(double x1, double y1, double x2, double y2)
{
	double calculation = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (double)2);
	return calculation;
}


// Function to evaluate the solutions and return the fitness value
double evaluate_solution(vector<int> pos, double** mat, int length)
{
	double sum = 0;
	double val;
	for (size_t i = 0; i <= (pos.size()-2); i++)
	{
		for (size_t j = i + 1; j <= (pos.size() - 1); j++)
		{
			val = mat[pos[i]][pos[j]];
			sum = sum + val;
		}
	}
	return sum;
}
tuple<double, int*> comb(int N, int K, double** distance_matrix)
{
	double fitness = DBL_MAX;
	double tempFitness;
	int count=0;
	int* best_solution = (int*)malloc(K * sizeof(int));
	vector <int> combination;
	string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's

	// print integers and permute bitmask
	do {
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) combination.push_back(i);
		}
		tempFitness = evaluate_solution(combination, distance_matrix, N);
		if (tempFitness < fitness)
		{
			fitness = tempFitness;
			for (size_t i = 0; i < combination.size(); i++)
			{
				best_solution[i] = combination[i];
			}
		}
		/*if (count % 10000 == 0) {
			cout << "Mejor Fitness:" << fitness<<endl;
			cout << "Iteracion:" << count<<endl;
		}*/
		combination.clear();
		combination.shrink_to_fit();
		count++;
	} while (prev_permutation(bitmask.begin(), bitmask.end()));
	cout << "Combinaciones:" << count<<endl;
	return { fitness,best_solution };
}
// TODO: Haga referencia aquí a los encabezados adicionales que el programa requiere.
