#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
 
using namespace std;
using Clock = chrono::high_resolution_clock;
 
// ============================================================
//  Contadores globales para comparaciones e intercambios
// ============================================================
long long g_comparaciones = 0;
long long g_intercambios  = 0;
 
// ============================================================
//  BUBBLE SORT  (estable, O(n^2) peor/promedio, O(n) mejor)
// ============================================================
void bubbleSort(vector<int>& arr) {
    int n = (int)arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            g_comparaciones++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                g_intercambios++;
                swapped = true;
            }
        }
        if (!swapped) break;   // optimizacion: lista ya ordenada
    }
}
 
// ============================================================
//  SELECTION SORT  (no estable, O(n^2) todos los casos)
// ============================================================
void selectionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            g_comparaciones++;
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
            g_intercambios++;
        }
    }
}
 
// ============================================================
//  INSERTION SORT  (estable, O(n^2) peor/promedio, O(n) mejor)
// ============================================================
void insertionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            g_comparaciones++;
            arr[j + 1] = arr[j];
            g_intercambios++;
            j--;
        }
        if (j >= 0) g_comparaciones++;  // la comparacion que fallo
        arr[j + 1] = key;
    }
}
 
// ============================================================
//  SHELL SORT  (no estable, O(n log^2 n) con secuencia Knuth)
// ============================================================
void shellSort(vector<int>& arr) {
    int n = (int)arr.size();
    // Secuencia de Knuth: 1, 4, 13, 40, 121, ...
    int gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
 
    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap) {
                g_comparaciones++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    g_intercambios++;
                    j -= gap;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
        gap /= 3;
    }
}
 
// ============================================================
//  Funcion de prueba
// ============================================================
struct Resultado {
    long long tiempo_us;
    long long comparaciones;
    long long intercambios;
};
 
Resultado test(const vector<int>& data,
               void (*func)(vector<int>&),
               const string& nombre) {
    vector<int> arr = data;
    g_comparaciones  = 0;
    g_intercambios   = 0;
 
    auto t1 = Clock::now();
    func(arr);
    auto t2 = Clock::now();
 
    Resultado r;
    r.tiempo_us    = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    r.comparaciones = g_comparaciones;
    r.intercambios  = g_intercambios;
    return r;
}
 
// ============================================================
//  Imprime una fila de la tabla
// ============================================================
void imprimirFila(const string& algoritmo,
                  const string& caso,
                  int n,
                  const Resultado& r) {
    cout << left  << setw(18) << algoritmo
         << setw(10) << caso
         << right << setw(8)  << n
         << setw(12) << r.tiempo_us
         << setw(15) << r.comparaciones
         << setw(15) << r.intercambios
         << "\n";
}
 
int main() {
    srand(42);  // semilla fija para reproducibilidad
 
    vector<int> tamanos = {10, 100, 1000};
 
    cout << "\n";
    cout << string(78, '=') << "\n";
    cout << "  BENCHMARK DE ALGORITMOS DE ORDENAMIENTO  (C++11)\n";
    cout << string(78, '=') << "\n";
    cout << left  << setw(18) << "Algoritmo"
         << setw(10) << "Caso"
         << right << setw(8)  << "n"
         << setw(12) << "Tiempo(us)"
         << setw(15) << "Comparaciones"
         << setw(15) << "Intercambios"
         << "\n";
    cout << string(78, '-') << "\n";
 
    for (int n : tamanos) {
        // Generar datos base aleatorios
        vector<int> aleatorio(n);
        for (int i = 0; i < n; i++) aleatorio[i] = rand() % 10000;
 
        // Mejor caso:  arreglo ya ordenado
        vector<int> mejor = aleatorio;
        sort(mejor.begin(), mejor.end());
 
        // Peor caso: arreglo en orden inverso
        vector<int> peor = mejor;
        reverse(peor.begin(), peor.end());
 
        // Casos y nombres
        vector<pair<string, vector<int>*>> casos = {
            {"Mejor",    &mejor},
            {"Promedio", &aleatorio},
            {"Peor",     &peor}
        };
 
        vector<pair<string, void(*)(vector<int>&)>> algoritmos = {
            {"Bubble Sort",    bubbleSort},
            {"Selection Sort", selectionSort},
            {"Insertion Sort", insertionSort},
            {"Shell Sort",     shellSort}
        };
 
        for (auto& alg : algoritmos) {
            for (auto& cas : casos) {
                Resultado r = test(*cas.second, alg.second, alg.first);
                imprimirFila(alg.first, cas.first, n, r);
            }
        }
        cout << string(78, '-') << "\n";
    }
 
    cout << "\nNotas:\n";
    cout << "  - Mejor caso   : arreglo ya ordenado ascendentemente\n";
    cout << "  - Promedio caso: arreglo con valores aleatorios (semilla=42)\n";
    cout << "  - Peor caso    : arreglo en orden descendente\n";
    cout << "  - Tiempo en microsegundos (us)\n";
    cout << "  - Compilado con C++11, optimizacion -O0\n\n";
 
    return 0;
}