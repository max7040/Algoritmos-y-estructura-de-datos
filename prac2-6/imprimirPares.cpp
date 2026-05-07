#include <iostream>
using namespace std;

void imprimirPares(int n) {
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            cout << i << endl;
        }
    }
}

int main() {
    int n;
    cout << "Ingrese un numero: ";
    cin >> n;
    imprimirPares(n);
    return 0;
}