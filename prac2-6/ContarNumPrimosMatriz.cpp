#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

vector<vector<int>> generarMatriz(int filas, int columnas) {
    vector<vector<int>> matriz(filas, vector<int>(columnas));

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = rand() % 101;
        }
    }

    return matriz;
}

bool esPrimo(int n) {
    if (n < 2) {
        return false;
    }

    if (n == 2) {
        return true;
    }

    if (n % 2 == 0) {
        return false;
    }

    int limite = sqrt(n);

    for (int i = 3; i <= limite; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int contarPrimos(const vector<vector<int>>& matriz) {
    int conteo = 0;

    for (const auto& fila : matriz) {
        for (int valor : fila) {
            if (esPrimo(valor)) {
                conteo++;
            }
        }
    }

    return conteo;
}

int main() {
    srand(time(0));

    int filas = 100;
    int columnas = 100;

    vector<vector<int>> matriz = generarMatriz(filas, columnas);

    clock_t inicio = clock();

    int primos = contarPrimos(matriz);

    clock_t fin = clock();

    double tiempo = double(fin - inicio) / CLOCKS_PER_SEC;

    cout << "Numeros primos: " << primos << endl;
    cout << "Tiempo de ejecucion: " << tiempo << " segundos" << endl;

    return 0;
}