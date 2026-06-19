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
		} else {
			Nodo* actual = cabeza;
			while (actual->siguiente != nullptr)
				actual = actual->siguiente;
			actual->siguiente = nuevo;
		}
	}
	void insertar ( int indice, int valor ){
	    if ( indice == 0 || cabeza == nullptr ){
	        insertarInicio( valor );
	    }//cd
	    else{
	        indice--;
	        
	        int posicion = 0;

	        Nodo *nuevo = new Nodo();

	        
	        nuevo->dato = valor;
	        
	        Nodo *actual = cabeza;

	        while ( true ){
	            if ( actual->siguiente == nullptr ){
	                nuevo->siguiente = nullptr;

	                
	                actual->siguiente = nuevo;
	                
	                break;
	            }//cd
	            else if ( posicion == indice ){
	                nuevo->siguiente = actual->siguiente;

	                actual->siguiente = nuevo;

	                break;
	            }//cd
	            
	            actual = actual->siguiente;
	            
	            posicion++;
	        }//wh
	    }//cd
	}//fnc
	void eliminarInicio() {
		if (cabeza != nullptr) {
			Nodo* temp = cabeza;
			cabeza = cabeza->siguiente;
			delete temp;
		}
	}
	void mostrar() {
		Nodo* actual = cabeza;
		while (actual != nullptr) {
			cout << actual->dato << " -> ";
			actual = actual->siguiente;
		}
		cout << "NULL" << endl;
	}
};
int main() {
	Lista lista;
	lista.insertarInicio(0);
	lista.insertarFinal(20);
	lista.insertarInicio(40);
	lista.insertarInicio(50);
	lista.insertarFinal(20);
	lista.eliminarInicio();
	lista.mostrar();
	lista.insertar ( 1, 7 );
	lista.mostrar(); // 20 -> 10 -> 30 -> NULL
	return ( 0 );
}