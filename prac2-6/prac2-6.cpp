#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

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

int contarPares(const vector<vector<int>>& matriz) {
    int conteo = 0;

    for (const auto& fila : matriz) {
        for (int val : fila) {
            if (val % 2 == 0) {
                conteo++;
            }
        }
    }
    return conteo;
}

int main() {
    srand(time(0));

    vector<pair<int,int>> tamanos = {
        {50,50},
        {100,100},
        {200,200}
    };

    for (auto tam : tamanos) {
        int filas = tam.first;
        int columnas = tam.second;

        vector<vector<int>> matriz = generarMatriz(filas, columnas);

        clock_t inicio = clock();
        int resultado = contarPares(matriz);
        clock_t fin = clock();

        double tiempo = double(fin - inicio) / CLOCKS_PER_SEC;

        cout << "Matriz: " << filas << "x" << columnas << endl;
        cout << "Numeros pares: " << resultado << endl;
        cout << "Tiempo de ejecucion: " << tiempo << " segundos" << endl;
        cout << "----------------------------------" << endl;
    }

    return 0;
}