#include <iostream>
#include <stdexcept>
using namespace std;

// Nodo genérico
template <class T> // nodo generico
struct Nodo {
    T dato; // dato que se guarda
    Nodo<T>* siguiente; // ptr al sgt nodo

    Nodo(T valor) { // consstructor al crear un nodo
        //Nodo<int>* nuevo = new Nodo<int>(50);
        dato = valor;
        siguiente = nullptr;
    }
};

// Cola dinámica
template <class T>
class ColaDinamica {
private:
    Nodo<T>* frente;
    Nodo<T>* fin;
    int cantidad;

public:
    // Constructor
    ColaDinamica() {
        frente = fin = nullptr;
        cantidad = 0;
    }

    // Destructor
    ~ColaDinamica() { // 
        while (!estaVacia()) { // cola NO VACIA
            desencolar(); // elimina nodo a nodo liberar memoria
        }
    }

    // Ver si está vacía
    bool estaVacia() const {
        return frente == nullptr;
    }

    // Tamaño
    int size() const {
        return cantidad;
    }

    // Encolar
    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (estaVacia()) {
            frente = fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }

        cantidad++;
    }

    // Desencolar
    T desencolar() {
        if (estaVacia()) {
            throw out_of_range("Cola vacia");
        }

        Nodo<T>* temp = frente;
        T valor = temp->dato;

        frente = frente->siguiente;

        if (frente == nullptr) {
            fin = nullptr;
        }

        delete temp;
        cantidad--;

        return valor;
    }

    // Ver frente
    T verFrente() const {
        if (estaVacia()) {
            throw out_of_range("Cola vacia");
        }

        return frente->dato;
    }

    // Ver final
    T verFin() const {
        if (estaVacia()) {
            throw out_of_range("Cola vacia");
        }

        return fin->dato;
    }

    // Mostrar
    void mostrar() const {
        if (estaVacia()) {
            cout << "Cola vacia\n";
            return;
        }

        Nodo<T>* actual = frente;

        cout << "Cola: ";
        while (actual != nullptr) {
            cout << actual->dato << " ";
            actual = actual->siguiente;
        }
        cout << endl;
    }
};

int main() {
    ColaDinamica<int> cola;// clase + tipo dato + obj

    try {
        // 🔹 Intentar ver frente en cola vacía
        cout << "Intentando ver el frente...\n";
        cout << cola.verFrente() << endl;

    } catch (const exception& e) {
        cout << "Excepcion capturada: " << e.what() << endl;
    }

    // Uso normal
    cola.encolar(10);
    cola.encolar(20);
    cola.encolar(30);

    cout << "\nCola despues de encolar:\n";
    cola.mostrar();

    //  Vaciar la cola
    cout << "\nVaciando cola...\n";
    while (!cola.estaVacia()) {
        cout << "Desencolado: " << cola.desencolar() << endl;
    }

    // otra excepción
    try {
        cout << "\nIntentando desencolar otra vez...\n";
        cout << cola.desencolar() << endl;
        cout << "parte no ejecutada";

    } catch (const exception& e) {
        cout << "Excepcion capturada: " << e.what() << endl;
    }

    return 0;
}