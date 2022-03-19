#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

int main() {

    std::ifstream archivo("ejemplo2.json");
    nlohmann::json data = nlohmann::json::parse(archivo);

    int m = 40;

    int cromosoma[40][154] = {};
    float fitnessPob[40] = {};

    int cromosoma_nuevo[40][154] = {};
    float fitnessPob_nuevo[40] = {};

    //float fitnessEvol[] = {};

    //int itera[] = {};

    //int crossover12[] = {};
    //int crossover21[] = {};
    //prueba funcionamiento arreglos
    cromosoma[0][0] = 1;
    fitnessPob[0] = 0.1;
    cromosoma_nuevo[0][0] = 2;
    //memoria XD (Buscar como trabajar bien con la memoria)
    std::cout << data["diputados"][0];
    return 0;
}
