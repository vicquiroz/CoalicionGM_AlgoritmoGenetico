#include "brute_force_algorithm.h"

#define pb push_back

using namespace std;
using json = nlohmann::json;

int main() {
    // Call the Json file
    ifstream file("votes.json");
    json data = json::parse(file);

    int n = data["votes"].size();
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

    vector <int> s(n);
    iota(s.begin(), s.end(), 0);
    map <vector<int>, int> m; // To check if we already have that variation
    vector <double> fitness;
    vector <vector <int>> v; // Variations
    do {
        vector <int> var;
        for (int i = 0; i < quorum; i++) var.pb(s[i]);
        sort(var.begin(), var.end(), &vector_sort);
        if (m[var] == 0) {
            m[var] = 1;
            v.pb(var);
        }
    } while (next_permutation(s.begin(), s.end()));
    for (vector<int> coalition:v )
    {
        fitness.pb(evaluate_solution(coalition, distance_matrix, quorum));
    }
    for (double currentFitness : fitness) {
        cout << currentFitness << endl;
    }
    int minIndex = distance(begin(fitness), min_element(begin(fitness), end(fitness)));
    cout << "Minimum Fitness:" << fitness[minIndex] << endl;
    cout << "Coalition:" << endl;
    for (size_t i = 0; i < quorum; i++)
    {
        cout << v[minIndex][i] << ",";
    }
    return 0;
}