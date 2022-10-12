#include "brute_force_algorithm.h"
#include <chrono>
#define pb push_back

using namespace std;
using json = nlohmann::json;

int main() {
    // Call the Json file
    ifstream file("votes.json");
    json data = json::parse(file);

    int n = data["votes"].size();
    n = 30;
    int quorum = trunc(n / 2)+1;
    // Initialize the distance matrix
    double** distance_matrix = (double**)malloc(n * sizeof(double*));
    for (size_t i = 0; i < n; i++)
    {
        distance_matrix[i] = (double*)malloc(n * sizeof(double));
    }

    // Fill the distance matrix
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            distance_matrix[i][j] = euclidian_distance(data["votes"][i]["x"], data["votes"][i]["y"], data["votes"][j]["x"], data["votes"][j]["y"]);
        }
    }
    // Time variable initialization for execution calculation
    auto initial_time = chrono::high_resolution_clock::now();
    cout << "Generando combinatoria!" << endl;
    double fitness;
    int* coalition = (int*)malloc(quorum * sizeof(int));
    tie(fitness,coalition)=comb(n, quorum,distance_matrix);
    cout << "Fin Combinatoria!" << endl;
    // Stop the clock
    auto final_time = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(final_time - initial_time).count();
    // Convert the time taken by the algorithm to seconds
    time_taken *= 1e-9;
    cout << "Time:"<< fixed << time_taken << setprecision(9) << endl;
    cout << "Minimum Fitness:" << fitness << endl;
    cout << "Coalition:" << endl;
    for (size_t i = 0; i < quorum; i++)
    {
        cout << coalition[i] << ",";
    }
    return 0;
}