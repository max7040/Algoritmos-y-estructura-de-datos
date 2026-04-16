#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;

// ======================================================
// BUBBLE SORT
// Ordena intercambiando elementos adyacentes
// ======================================================
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            // Si el elemento actual es mayor al siguiente, se intercambian
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // Si no hubo intercambios, ya está ordenado
        if (!swapped) break;
    }
}

// ======================================================
// SELECTION SORT
// Busca el mínimo y lo coloca en su posición correcta
// ======================================================
void selectionSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;

        for (int j = i + 1; j < n; j++) {
            // Busca el índice del valor mínimo
            if (arr[j] < arr[minIdx]) minIdx = j;
        }

        // Intercambia el mínimo con la posición actual
        swap(arr[i], arr[minIdx]);
    }
}

// ======================================================
// INSERTION SORT
// Inserta cada elemento en su posición correcta
// ======================================================
void insertionSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Mueve los elementos mayores hacia la derecha
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        // Inserta el elemento en su posición correcta
        arr[j + 1] = key;
    }
}

// ======================================================
// SHELL SORT
// Mejora insertion sort usando saltos (gaps)
// ======================================================
void shellSort(vector<int>& arr) {
    int n = arr.size();

    // Se inicia con un gap grande y se reduce progresivamente
    for (int gap = n / 2; gap > 0; gap /= 2) {

        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            // Inserción con salto
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
    }
}

// ======================================================
// FUNCIÓN DE PRUEBA
// Mide el tiempo de ejecución de cada algoritmo
// ======================================================
void test(vector<int> data, void(*func)(vector<int>&), const string& name) {
    auto arr = data;

    auto t1 = Clock::now();   // Inicio
    func(arr);
    auto t2 = Clock::now();   // Fin

    // Muestra el tiempo en microsegundos
    cout << name << " took "
         << chrono::duration_cast<chrono::microseconds>(t2 - t1).count()
         << " μs\n";
}

// ======================================================
// FUNCIÓN PRINCIPAL
// Genera datos aleatorios y ejecuta los algoritmos
// ======================================================
int main() {
    vector<int> sizes = {10, 100, 1000};

    for (int n : sizes) {
        vector<int> data(n);

        // Genera números aleatorios
        for (int i = 0; i < n; i++) {
            data[i] = rand();
        }

        cout << "--- Size: " << n << " ---\n";

        // Ejecuta cada algoritmo
        test(data, bubbleSort, "Bubble Sort");
        test(data, selectionSort, "Selection Sort");
        test(data, insertionSort, "Insertion Sort");
        test(data, shellSort, "Shell Sort");
    }

    return 0;
}