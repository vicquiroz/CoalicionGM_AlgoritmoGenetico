#include <iostream>
#include <array>
#include <random>
#include <fstream>
#include <string>
#include <map>
using namespace std;
default_random_engine generator;
uniform_real_distribution<double> uniform;
normal_distribution<double> normal;
random_device rng;
mt19937 mt{ rng() };
const double pi = acos(-1);
double euclidian_distance(double x1, double y1, double x2, double y2)
{
	double calculation = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (double)2);
	return calculation;
}

void generate_uniform_distribution(int sample_size) {
	ofstream file;
	file.open("votes.json");
	string votes = "{\"votes\":[\n";
	uniform = uniform_real_distribution<double>(-1, 1);
	double value_x;
	double value_y;
	for (size_t i = 0; i < sample_size; i++)
	{
		do {
			value_x = uniform(mt);
			value_y = uniform(mt);
		} while (euclidian_distance(value_x, value_y, 0, 0) > 1);
		votes = votes + "    " + "{\"x\":" + to_string(value_x) + "," + "\"y\":" + to_string(value_y) + "},\n";
	}
	replace(votes.end() - 2, votes.end(), ',', ' ');
	votes = votes + "]}";
	cout << "Done!";
	file << votes;
}

void generate_normal_distribution(int sample_size, int rotacion) {
	ofstream file;
	file.open("votes.json");
	string votes = "{\"votes\":[\n";
	normal = normal_distribution<double>(0, 1);
	double value_x;
	double value_y;
	double temp_x;
	double temp_y;
	double radian_rot = (rotacion*pi)/180;
	for (size_t i = 0; i < sample_size; i++)
	{
		do {
			value_x = normal(mt);
			value_y = normal(mt);
			temp_x = value_x;
			temp_y = value_y;
			value_x = temp_x * cos(radian_rot) + temp_y * sin(radian_rot);
			value_y = -temp_x * sin(radian_rot) + temp_y * cos(radian_rot);

		} while (euclidian_distance(value_x, value_y, 0, 0) > 1);
		votes = votes + "    " + "{\"x\":" + to_string(value_x) + "," + "\"y\":" + to_string(value_y) + "},\n";
	}
	replace(votes.end() - 2, votes.end(), ',', ' ');
	votes = votes + "]}";
	cout << "Done!";
	file << votes;
}

void generate_uniform_binomial_distribution(int sample_size){
	ofstream file;
	file.open("votes.json");
	string votes = "{\"votes\":[\n";
	uniform = uniform_real_distribution<double>(-1, 1);
	array<double, 11> intervals{ -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };
	array<double, 10> weights{ 1.0, 2.0, 2.0, 1.0, 0.5, 1.0, 2.0, 2.0, 1.0, 0.5 };
	piecewise_constant_distribution<double> distribution(intervals.begin(), intervals.end(), weights.begin());
	double value_x;
	double value_y;
	for (size_t i = 0; i < sample_size; i++)
	{
		do {
			value_x = distribution(generator);
			value_y = uniform(mt);
		} while (euclidian_distance(value_x, value_y, 0, 0) > 1);
		votes = votes + "    " + "{\"x\":" + to_string(value_x) + "," + "\"y\":" + to_string(value_y) + "},\n";
	}
	replace(votes.end() - 2, votes.end(), ',', ' ');
	votes = votes + "]}";
	cout << "Done!";
	file << votes;
}

void generate_binomial_distribution(int sample_size) {
	ofstream file;
	file.open("votes.json");
	string votes = "{\"votes\":[\n";
	gamma_distribution<double> gamma(2.0, 0.5);
	array<double, 11> intervals{ -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };
	array<double, 10> weights{ 1.0, 2.0, 2.0, 1.0, 0.5, 1.0, 2.0, 2.0, 1.0, 0.5 };
	piecewise_constant_distribution<double> distribution(intervals.begin(), intervals.end(), weights.begin());
	double value_x;
	double value_y;
	for (size_t i = 0; i < sample_size; i++)
	{
		do {
			value_x = distribution(generator);
			value_y = gamma(generator)-1;
		} while (euclidian_distance(value_x, value_y, 0, 0) > 1);
		votes = votes + "    " + "{\"x\":" + to_string(value_x) + "," + "\"y\":" + to_string(value_y) + "},\n";
	}
	replace(votes.end() - 2, votes.end(), ',', ' ');
	votes = votes + "]}";
	cout << "Done!";
	file << votes;
}