#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> cola;

    cola.push(3);
    cola.push(6);
    cola.push(9);
    cola.push(12);

    cola.pop();

    cout << "Frente actual: " << cola.front() << endl;

    return 0;
}