#include <iostream>
#define MAX 100

using namespace std;

class Pila {
private:
    int datos[MAX];
    int tope;

public:
    Pila() {
        tope = -1;
    }

    bool estaVacia() {
        return tope == -1;
    }

    bool estaLlena() {
        return tope == MAX - 1;
    }

    void apilar(int valor) {
        if (estaLlena()) {
            cout << "Pila llena\n";
            return;
        }

        datos[++tope] = valor;
    }

    void desapilar() {
        if (estaVacia()) {
            cout << "Pila vacía\n";
            return;
        }

        tope--;
    }

    int cima() {
        if (!estaVacia())
            return datos[tope];

        return -1;
    }

    void mostrar() {
        for (int i = tope; i >= 0; i--)
            cout << datos[i] << " ";

        cout << endl;
    }
};

int main() {

    Pila pila;

    pila.apilar(12);
    pila.apilar(25);
    pila.apilar(37);
    pila.apilar(48);
    pila.apilar(59);
    
    pila.mostrar(); // 59 48 37 25 12

    pila.desapilar();

    pila.mostrar(); // 48 37 25 12

    cout << "Cima: " << pila.cima() << endl;

    return 0;
}