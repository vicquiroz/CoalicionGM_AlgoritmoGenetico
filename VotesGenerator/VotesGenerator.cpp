#include "VotesGenerator.h"
#include <random>
#include <fstream>
#include <string>
using namespace std;
uniform_real_distribution<double> uniform;
random_device rng;
mt19937 mt{ rng() };
int sample_size = 20;
int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		sample_size = stoi(argv[1]);
	}
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
		votes=votes+"    "+ "{\"x\":" + to_string(value_x) + "," + "\"y\":" + to_string(value_y) + "},\n";
	}
	replace(votes.end() - 2, votes.end(), ',', ' ');
	votes = votes + "]}";
	cout << "Done!";
	file << votes;
}
