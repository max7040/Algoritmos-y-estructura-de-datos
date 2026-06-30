/*
 * Practica - Algoritmos y Estructuras de Datos
 * Tabla Hash con manejo de colisiones por Encadenamiento
 *
 * Compilar:  g++ -std=c++17 -O2 -o hash_table hash_table.cpp
 * Ejecutar:  ./hash_table
 */

#include <iostream>
#include <list>
#include <vector>
#include <chrono>

using namespace std;

class HashTable {
private:
    int buckets;
    vector<list<int>> table;

    int hashFunction(int key) const {
        return key % buckets;
    }

public:
    explicit HashTable(int size) : buckets(size), table(size) {}

    void insertItem(int key) {
        int idx = hashFunction(key);
        table[idx].push_back(key);
    }

    bool searchItem(int key) const {
        int idx = hashFunction(key);
        for (int val : table[idx]) {
            if (val == key) return true;
        }
        return false;
    }

    void removeItem(int key) {
        int idx = hashFunction(key);
        table[idx].remove(key);
    }

    // Numero de colisiones = elementos que comparten un bucket con al menos otro
    int countCollisions() const {
        int collisions = 0;
        for (const auto &bucket : table) {
            if (bucket.size() > 1) collisions += static_cast<int>(bucket.size()) - 1;
        }
        return collisions;
    }

    // Tamano del bucket mas cargado (factor de carga maximo)
    size_t maxBucketSize() const {
        size_t maxSize = 0;
        for (const auto &bucket : table) maxSize = max(maxSize, bucket.size());
        return maxSize;
    }

    void display() const {
        for (int i = 0; i < buckets; i++) {
            cout << i << ": ";
            for (int val : table[i]) cout << val << " -> ";
            cout << "NULL\n";
        }
    }
};

// Ejecuta una bateria de pruebas sobre una tabla de "size" buckets
// usando el mismo conjunto de claves, y reporta colisiones y tiempo.
void analizarRendimiento(int size, const vector<int> &claves) {
    HashTable ht(size);

    auto start = chrono::high_resolution_clock::now();
    for (int k : claves) ht.insertItem(k);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    cout << "Tamano de tabla: " << size
         << " | Colisiones: " << ht.countCollisions()
         << " | Bucket mas cargado: " << ht.maxBucketSize()
         << " | Tiempo insercion: " << ms << " ms\n";
}

int main() {
    cout << "=== 1. Demostracion basica (tabla de 7 buckets) ===\n";
    HashTable ht(7);
    ht.insertItem(15);
    ht.insertItem(11);
    ht.insertItem(27);
    ht.insertItem(8);
    ht.insertItem(12);
    ht.display();
    cout << "Existe 27? " << (ht.searchItem(27) ? "Si" : "No") << endl;

    cout << "\n=== 2. Analisis de rendimiento variando el tamano de la tabla ===\n";
    // Mismo conjunto de 20 claves (pseudo-aleatorias) probado contra distintos
    // tamanos de tabla, para comparar colisiones de forma justa.
    vector<int> claves = {482, 19, 357, 1041, 88, 673, 290, 14, 905, 56,
                           738, 221, 9, 460, 187, 32, 815, 503, 71, 994};

    analizarRendimiento(5, claves);
    analizarRendimiento(7, claves);
    analizarRendimiento(11, claves);
    analizarRendimiento(23, claves);   // numero primo, mas cercano a 20 elementos
    analizarRendimiento(101, claves);  // tabla mucho mas grande -> menos colisiones

    cout << "\nConclusion: al aumentar el numero de buckets (especialmente usando\n"
         << "numeros primos) se reduce la cantidad de colisiones y el tamano del\n"
         << "bucket mas cargado, acercando el tiempo de busqueda/insercion a O(1).\n";

    return 0;
}
