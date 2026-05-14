#include <iostream>
using namespace std;

class Pila {
private:
    int arr[100];
    int tope;

public:
    Pila() {
        tope = -1;
    }

    void push(int valor) {
        tope++;
        arr[tope] = valor;
    }

    void pop() {
        if (tope >= 0) {
            tope--;
        }
    }

    int top() {
        return arr[tope];
    }

    void mostrar() {
        for(int i = tope; i >= 0; i--) {
            cout << arr[i] << endl;
        }
    }
};

int main() {
    Pila p;

    p.push(5);
    p.push(10);
    p.push(15);
    p.push(20);
    p.push(25);

    p.pop();
    p.pop();

    cout << "Tope actual: " << p.top() << endl;

    cout << "Contenido de la pila:" << endl;
    p.mostrar();

    return 0;
}