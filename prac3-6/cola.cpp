#include <iostream>
using namespace std;

class Cola {
private:
    int arr[100];
    int frente;
    int final;

public:
    Cola() {
        frente = 0;
        final = -1;
    }

    void enqueue(int x) {
        final++;
        arr[final] = x;
    }

    void dequeue() {
        if (frente <= final) {
            frente++;
        }
    }

    void mostrar() {
        for(int i = frente; i <= final; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Cola cola;

    cola.enqueue(5);
    cola.enqueue(10);
    cola.enqueue(15);
    cola.enqueue(20);
    cola.enqueue(25);

    cola.dequeue();
    cola.dequeue();

    cola.mostrar();

    return 0;
}