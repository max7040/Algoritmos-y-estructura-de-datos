#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

class Lista {
private:
    Nodo* cabeza;

public:
    Lista() {
        cabeza = nullptr;
    }

    void insertarInicio(int valor) {
        Nodo* nuevo = new Nodo();
        nuevo->dato = valor;
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    void insertarFinal(int valor) {
        Nodo* nuevo = new Nodo();
        nuevo->dato = valor;
        nuevo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevo;
            return;
        }

        Nodo* temp = cabeza;

        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        temp->siguiente = nuevo;
    }

    void eliminarInicio() {
        if (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void mostrar() {
        Nodo* temp = cabeza;

        while (temp != nullptr) {
            cout << temp->dato << " -> ";
            temp = temp->siguiente;
        }

        cout << "NULL" << endl;
    }
};

int main() {
    Lista lista;

    lista.insertarInicio(8);
    lista.insertarInicio(4);
    lista.insertarFinal(11);

    lista.eliminarInicio();

    lista.mostrar();

    return 0;
}