#include <iostream>
#include <stdexcept> // para excepciones
using namespace std;

template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) {
        dato = valor;
        siguiente = nullptr;
    }
};

template <typename T>
class Lista {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;

public:
    Lista() {
        cabeza = nullptr;
        cola = nullptr;
    }

    ~Lista() {
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void insertarInicio(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (!nuevo) {
            throw runtime_error("Error: no se pudo asignar memoria");
        }

        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        }
    }

    void insertarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (!nuevo) {
            throw runtime_error("Error: no se pudo asignar memoria");
        }

        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
    }

    void eliminarInicio() {
        if (cabeza == nullptr) {
            throw runtime_error("Error: lista vacía, no se puede eliminar");
        }

        Nodo<T>* temp = cabeza;
        cabeza = cabeza->siguiente;

        if (cabeza == nullptr) {
            cola = nullptr;
        }

        delete temp;
    }

    void mostrar() {
        if (cabeza == nullptr) {
            throw runtime_error("Error: lista vacía");
        }

        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            cout << actual->dato << " -> ";
            actual = actual->siguiente;
        }
        cout << "NULL\n";
    }
};

int main() {
    try {
        Lista<int> lista;

        lista.insertarInicio(10);
        lista.insertarInicio(20);
        lista.insertarFinal(30);

        lista.mostrar();

        lista.eliminarInicio();
        lista.mostrar();

        // Prueba de error
        lista.eliminarInicio();
        lista.eliminarInicio();
        lista.eliminarInicio(); // aquí lanza excepción

    } catch (const exception& e) {
        cout << "⚠️ Excepción capturada: " << e.what() << endl;
    }

    return 0;
}