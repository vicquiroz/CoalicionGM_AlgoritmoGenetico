// brute_force_algorithm.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos de un proyecto.

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <math.h>
#include <iostream>
#include <nlohmann/json.hpp>
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

// TODO: Haga referencia aquí a los encabezados adicionales que el programa requiere.
