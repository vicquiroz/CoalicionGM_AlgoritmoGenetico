#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <math.h>
#include <random>

float dis_euc(float x1, float y1, float x2, float y2) {
    float calculo = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / 2);
    return ;
}

float suma(float array[]) {
    int suma = 0;
    for (size_t i = 0; i < sizeof(array); i++){
        suma=suma+array[i];
    }
    return suma;
}

float suma_bool(float array[]) {
    int suma = 0;
    for (size_t i = 0; i < sizeof(array); i++)
    {
        if (array[i]) {
            suma++;
        }
    }
    return suma;
}

float eval_sol(float pos[], nlohmann::json data) {
    float suma = 0;
    for (size_t i = 0; i < (sizeof(pos) - 1); i++)
    {
        for (size_t j = i + 1; j < sizeof(pos); j++)
        {
            suma = suma + dis_euc(data["diputados"][pos[i]]["coordX"], data["diputados"][pos[i]]["coordY"], data["diputados"][pos[j]]["coordX"], data["diputados"][pos[j]]["coordY"]);
        }
    }
    return suma;
}

float smallest_greater(float seq[], float value) {
    int i = 0;
    bool flag = true;
    while (flag)
    {
        i++;
        if (seq[i] > value) {
            return i;
        }
        else {
            return NULL;
        }
    }
}

//se llama sample<cant>(limite)
template<std::size_t cant>
float* sample(int limite) {
    int valores[cant] = {};
    int i = 0;
    bool repetido = false;
    while (i < cant) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0, limite);
        int valor = uni(rng);
        for (size_t j = 0; j < sizeof(valores); j++)
        {
            if (valor == valores[j])repetido = true;
        }
        if (repetido == false) {
            valores[i] = valor;
            i++
        }
        repetido = false;
    }
    return valores;
}

int* sort(int* array, int n)
{
    int n = sizeof(array);
    for (size_t i = 0; i < n; i++)
    {
        bool already_sorted = true;
        for (size_t j = 0; j < n - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);
                already_sorted = false;
            }
        }
        if (already_sorted)
            break;
    }
    return array;
}

float* order(float* array[]) {
    int n = sizeof(array[1]);
    bool already_sorted = true;
    for (size_t i = 0; i < n; i++)
    {
        already_sorted = true;
        for (size_t j = 0; j < (n - i - 1); j++)
        {
            if (array[1][j] > array[1][j + 1])
            {
                array[0][j], array[0][j + 1] = array[0][j + 1], array[0][j];
                array[1][j], array[1][j + 1] = array[1][j + 1], array[1][j];
                already_sorted = false;
            }
        }
        if (already_sorted) break;
    }
    return (array[0], array[1]);
}

int* notin(int* array1, int* array2)
{
    const int n1 = sizeof(array1);
    const int n2 = sizeof(array2);
    int result[n1];
    bool flag = false;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            if (array1[i] == array2[j])
            {
                flag = true;
            }
        }
        if (flag == false)
        {
            result[i] = array1[i];
        }
        flag = false;
    }

    return result;
}

//Llamada de in_boolean<cant_b>(array1,array2)
template<std::size_t cant_b>
bool* in_boolean(float array1[], float array2[]) {
    bool result[] = new bool[cant_b];
    int result_len = 0;
    bool flag = false;
    for (size_t i = 0; i < sizeof(array1); i++)
    {
        for (size_t j = 0; j < sizeof(array2); j++)
        {
            if (array1[i] == array2[j])
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

float minimo(float array[]) {
    float mini = 0;
    for (size_t i = 0; i < sizeof(array)-1; i++)
    {
        if (array[i] < array[i + 1])mini = array[i];
        else mini = array[i + 1];
    }
    return mini;
}

float maximo(float array[]) {
    float maxi = 0;
    for (size_t i = 0; i < sizeof(array) - 1; i++)
    {
        if (array[i] > array[i + 1])maxi = array[i];
        else maxi = array[i + 1];
    }
    return maxi;
}

int which_max(float array[]) {
    int maxi = 0;
    for (size_t i = 0; i < sizeof(array) - 1; i++)
    {
        if (array[i] > array[i + 1]) {
            maxi = array[i];
        }
        else {
            maxi = array[i + 1];
        }
    }
    for (size_t i = 0; i < sizeof(array); i++)
    {
        if (array[i] == maxi) return i;
    }
}

int* which(bool array[]) {
    int cont = 0;
    for (size_t i = 0; i < sizeof(array); i++)
    {
        if (array[i]) cont++;
    }
    int* result = new int[cont];
    cont = 0;
    for (size_t i = 0; i < sizeof(array); i++)
    {
        if (array[i]) {
            result[cont] = i;
            cont++;
        }
    }
    return result;
}

//Llamada de crear_arreglo<numero>
template<std::size_t numero>
int* crear_arreglo()
{
    int arreglo[numero];
    for (size_t i = 0; i < numero; i++)
    {
        arreglo[i] = i;
    }

    return arreglo;
}

float* repetidos(float lista[]) {
    int count = 0;
    for (size_t i = 0; i < sizeof(lista); i++)
    {
        for (size_t j = 0; j < sizeof(lista); j++)
        {
            if (lista[j] == lista[i]) count++;
        }
        if (count > 1) {
            for (size_t k = i; k < sizeof(lista) - 1; k++)
            {
                lista[k] = lista[k + 1];
            }
        }
    }
    return lista;
}

//se llama sample_arreglo<cant>(array)
template<std::size_t cant>
float* sample_arreglo(float array[]) {
    int valores[cant] = {};
    int i = 0;
    bool repetido = false;
    while (i < cant) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0, sizeof(array)-1);
        int indice = uni(rng);
        float valor = array[indice];
        for (size_t j = 0; j < sizeof(valores); j++)
        {
            if (valor == valores[j])repetido = true;
        }
        if (repetido == false) {
            valores[i] = valor;
            i++
        }
        repetido = false;
    }
    return valores;
}

void main() {

    std::ifstream archivo("ejemplo2.json");
    nlohmann::json data = nlohmann::json::parse(archivo);

    const int m = 40;

    int cromosoma[m][154] = {};
    float fitnessPob[m] = {};

    int cromosoma_nuevo[m][154] = {};
    float fitnessPob_nuevo[m] = {};

    //float fitnessEvol[] = {};

    //int itera[] = {};

    //int crossover12[] = {};
    //int crossover21[] = {};
    //prueba funcionamiento arreglos
    cromosoma[0][0] = 1;
    fitnessPob[0] = 0.1;
    cromosoma_nuevo[0][0] = 2;
    //memoria XD (Buscar como trabajar bien con la memoria)
    std::cout << data["diputados"][0]["Nombre"];
}
