#include <iostream>
#include <stdexcept> // para excepciones
using namespace std;

template <typename T>// T tipo de dato generico
class Pila {
private:
    T* datos;
    int tope;
    int capacidad;

public:
    // Constructor
    Pila(int cap = 100) {
        capacidad = cap;
        datos = new T[capacidad];//nuevo array
        tope = -1;
    }

    // Destructor
    ~Pila() {
        delete[] datos;
    }

    // Verificar si está vacía
    bool estaVacia() const {
        return tope == -1;
    }

    // Verificar si está llena
    bool estaLlena() const {
        return tope == capacidad - 1;
    }

    // PUSH (apilar)
    void push(T valor) {
        if (estaLlena()) { // validacion
            throw overflow_error("Error: Pila llena (overflow)"); // no usa return
        }
        datos[++tope] = valor;
    }

    // POP (desapilar)
    void pop() {
        if (estaVacia()) {
            throw underflow_error("Error: Pila vacía (underflow)");// no usa return
        }
        tope--;
    }

    // TOP (cima)
    T top() const {
        if (estaVacia()) {
            throw underflow_error("Error: Pila vacía");
        }
        return datos[tope];
    }

    // Mostrar pila
    void mostrar() const {
        if (estaVacia()) { 
            cout << "Pila vacía\n";
            return;
        }

        for (int i = tope; i >= 0; i--) {
            cout << datos[i] << " ";
        }
        cout << endl;
    }

    // Tamaño actual
    int size() const {
        return tope + 1;
    }
};

int main() {
    try {
        Pila<int> p(5);// crea la pila

        p.push(10);
        p.push(20);
        p.push(30);

        p.mostrar(); // 30 20 10

        cout << "Cima: " << p.top() << endl;

        p.pop();
        p.mostrar(); // 20 10

        // Provocar error
        p.pop();
        p.pop();
        p.pop(); // 💥 excepción
        
        cout << "Hola"; // ❌ NUNCA se ejecuta

    } catch (exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}