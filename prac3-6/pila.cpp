#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> pila;

    pila.push(5);
    pila.push(10);
    pila.push(15);
    pila.push(20);
    pila.push(25);

    pila.pop();
    pila.pop();

    cout << "Tope actual: " << pila.top() << endl;

    return 0;
}