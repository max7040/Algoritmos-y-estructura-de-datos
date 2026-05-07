#include <iostream>
#include <ctime>
using namespace std;

// Funcion que calcula la suma usando un bucle
long long sumaBucle(long long n) {
    long long suma = 0;

    for (long long i = 1; i <= n; i++) {
        suma += i;
    }

    return suma;
}

// Funcion que calcula la suma usando formula directa
long long sumaFormula(long long n) {
    return n * (n + 1) / 2;
}

int main() {
    long long valores[] = {1000, 10000, 100000, 1000000};

    for (long long n : valores) {

        clock_t inicio, fin;

        inicio = clock();
        long long resultado1 = sumaBucle(n);
        fin = clock();

        double tiempoBucle =
            double(fin - inicio) / CLOCKS_PER_SEC;

        inicio = clock();
        long long resultado2 = sumaFormula(n);
        fin = clock();

        double tiempoFormula =
            double(fin - inicio) / CLOCKS_PER_SEC;

        cout << "N = " << n << endl;

        cout << "Resultado bucle: "
             << resultado1 << endl;

        cout << "Tiempo bucle: "
             << tiempoBucle
             << " segundos" << endl;

        cout << "Resultado formula: "
             << resultado2 << endl;

        cout << "Tiempo formula: "
             << tiempoFormula
             << " segundos" << endl;

        cout << "------------------------"
             << endl;
    }

    return 0;
}