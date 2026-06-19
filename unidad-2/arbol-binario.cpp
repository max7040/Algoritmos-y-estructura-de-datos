#include <iostream>

using namespace std;
struct Nodo {
    int dato;
    Nodo* izquierda;
    Nodo* derecha; 
};
Nodo* crearNodo(int valor) {
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->dato = valor;
    nuevoNodo->izquierda = nullptr;
    nuevoNodo->derecha = nullptr;
    return nuevoNodo;
}
void insertarNodo(Nodo* raiz, int valor) {
    if (valor < raiz->dato) {
        if (raiz->izquierda == nullptr) {
            raiz->izquierda = crearNodo(valor);
        } else {
            insertarNodo(raiz->izquierda, valor);
        }
    } else {
        if (raiz->derecha == nullptr) {
            raiz->derecha = crearNodo(valor);
        } else {
            insertarNodo(raiz->derecha, valor);
        }
    }
}
int main() {
    Nodo* raiz = crearNodo(10);
    int opc;
    do{
        cout << "1.- Ingrese un nodo: ";
        cout << "0.- Salir" << endl;
        cin >> opc;
        switch(opc){
            case 1:
                int valor;
                cout << "Ingrese el valor del nodo: ";
                cin >> valor;
                insertarNodo(raiz, valor);
                break;
            case 0:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
        }
    }while(opc != 0);

    return 0;
}