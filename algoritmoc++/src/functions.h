// Calling libraries
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

using namespace std;
// Initializing the seed
int seed = 2417704;

// Initializing the random number generator
uniform_int_distribution<int> uniform_1;
uniform_real_distribution<double> uniform_2;
uniform_int_distribution<int> pop_selection;
random_device rng;
default_random_engine generator(seed);
mt19937 mt{rng()};

// Initializing the struct of the solutions
struct Solution_structure
{
	int *coalition;
	double fitness;
};

// Boolean functions to check if a vector or array is sorted
bool array_sort(int const &lvd, int const &rvd)
{
	return lvd < rvd;
}
bool vector_initial_solutions_sort(Solution_structure const &lvd, Solution_structure const &rvd)
{
	return lvd.fitness < rvd.fitness;
}

// Function to calculate the distance between two points
double euclidian_distance(double x1, double y1, double x2, double y2)
{
	double calculation = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (double)2);
	return calculation;
}

// Function to evaluate the solutions and return the fitness value
double evaluate_solution(int *pos, double **mat, int length)
{
	double sum = 0;
	for (size_t i = 0; i <= (length - 2); i++)
	{
		for (size_t j = i + 1; j <= (length - 1); j++)
		{
			sum = sum + mat[pos[i]][pos[j]];
		}
	}
	return sum;
}

// Function to sum the values of an array_of_int
double summation(double *array, int length)
{
	double sum = 0;
	for (size_t i = 0; i <= length; i++)
	{
		sum = sum + array[i];
	}
	return sum;
}

// Function to get the number of boolean "TRUE" in an array
int summation_of_booleans(bool *array, int length)
{
	int sum = 0;
	for (size_t i = 0; i < length; i++)
	{
		if (array[i])
		{
			sum++;
		}
	}
	return sum;
}

// Function to get the position of the first value in an array that is smaller than a given value
int smallest_greater(double *seq, int length, double value)
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
			if (i == length)
			{
				break;
			}
		}
	}
	return (int)EXIT_FAILURE;
}

// Function to fill an array with random numbers between a given range
void sample(int *array_of_int, int limit, int length)
{
	int i = 0;
	bool repeated = false;
	int value = 0;
	if (limit == 0)
	{
		uniform_1 = uniform_int_distribution<int>(0, limit);
	}
	else
	{
		uniform_1 = uniform_int_distribution<int>(0, limit - 1);
	}
	while (i < length)
	{
		value = uniform_1(mt);
		for (size_t j = 0; j < i; j++)
		{
			if (value == array_of_int[j])
				repeated = true;
		}
		if (repeated == false)
		{
			array_of_int[i] = value;
			i++;
		}
		repeated = false;
	}
}

// Function to sort an array from smallest to largest and apply those changes to a matrix
void order(double *fitness, int **chromosome, int quorum, int sample_size)
{
	double temporal_variable = 0;
	int *arrTemp = (int *)malloc(quorum * sizeof(int));
	for (size_t i = 0; i < sample_size; i++)
	{
		bool already_sorted = true;
		for (size_t j = 0; j < sample_size - i - 1; j++)
		{
			if (fitness[j] > fitness[j + 1])
			{
				temporal_variable = fitness[j];
				fitness[j] = fitness[j + 1];
				fitness[j + 1] = temporal_variable;

				memcpy(arrTemp, chromosome[j], sizeof(int) * quorum);
				memcpy(chromosome[j], chromosome[j + 1], sizeof(int) * quorum);
				memcpy(chromosome[j + 1], arrTemp, sizeof(int) * quorum);
				already_sorted = false;
			}
		}
		if (already_sorted)
			break;
	}
	free(arrTemp);
}

// Function to return the values that are in array1 but not in array2 (values)
int **not_in(int *array_1, int *array_2, int length_1, int length_2)
{
	int *temporal_variable = (int *)malloc(sizeof(int) * length_1);
	int counter_1 = 0;
	bool flag = false;
	for (int i = 0; i < length_1; i++)
	{
		for (int j = 0; j < length_2; j++)
		{
			if (array_1[i] == array_2[j])
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			temporal_variable[counter_1] = array_1[i];
			counter_1++;
		}
		flag = false;
	}
	int **indexes = (int **)malloc(sizeof(int *) * 2);
	indexes[0] = (int *)malloc(sizeof(int) * counter_1);
	indexes[1] = (int *)malloc(sizeof(int) * 1);
	indexes[1][0] = counter_1;
	for (size_t i = 0; i < counter_1; i++)
	{
		indexes[0][i] = temporal_variable[i];
	}

	free(temporal_variable);
	return indexes;
}

// Function to return an array of booleans, where TRUE means that they are in both arrays and FALSE means that it is in the array1 but not in the array2
bool *in_boolean(int *array_1, int *array_2, int length_1, int length_2)
{
	bool *result = (bool *)malloc(sizeof(bool) * length_1);
	int result_len = 0;
	bool flag = false;
	for (size_t i = 0; i < length_1; i++)
	{
		for (size_t j = 0; j < length_2; j++)
		{
			if (array_1[i] == array_2[j])
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

// Function to return the smallest value of an array
int minimum_of_array(int *array, int length)
{
	int index_of_minimum_of_array = 0;
	for (size_t i = 0; i < length - 1; i++)
	{
		if (array[i] < array[i + 1])
			index_of_minimum_of_array = array[i];
		else
			index_of_minimum_of_array = array[i + 1];
	}
	return index_of_minimum_of_array;
}

// Function that given an array of booleans returns the position of the values that are TRUE
int **which(bool *array, int length)
{
	int counter_1 = 0;
	for (size_t i = 0; i < length; i++)
	{
		if (array[i])
			counter_1++;
	}
	int **indexes = nullptr;
	indexes = (int **)malloc(sizeof(int *) * 2);
	indexes[0] = (int *)malloc(sizeof(int) * counter_1);
	indexes[1] = (int *)malloc(sizeof(int));
	int counter_2 = 0;
	for (size_t i = 0; i < length; i++)
	{
		if (array[i])
		{
			indexes[0][counter_2] = i;
			counter_2++;
		}
	}
	indexes[1][0] = counter_2;
	return indexes;
}

// Function that creates and returns an array integers
int *create_array(int length)
{
	int *array_of_int = (int *)malloc(sizeof(int) * length);
	for (size_t i = 0; i < length; i++)
	{
		array_of_int[i] = i;
	}

	return array_of_int;
}

// Function that given an array is filled with random values
void sample_array(int *array_of_int, int quantity, int *valores, int length)
{
	int index = 0;
	int value = 0;
	int i = 0;
	bool repeated = false;
	uniform_1 = uniform_int_distribution<int>(0, length - 1);
	while (i < quantity)
	{
		index = uniform_1(mt);
		value = valores[index];
		for (size_t j = 0; j < i; j++)
		{
			if (value == array_of_int[j])
				repeated = true;
		}
		if (repeated == false)
		{
			array_of_int[i] = value;
			i++;
		}
		repeated = false;
	}
}
