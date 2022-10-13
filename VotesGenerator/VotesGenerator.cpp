#include "VotesGenerator.h"
using namespace std;
int sample_size = 450;
int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		sample_size = stoi(argv[1]);
	}
	generate_normal_distribution(sample_size,45);
}
