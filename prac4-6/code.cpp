#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// ============================
// CLASE NODO
// ============================

class Nodo {

public:

    int id;
    string nombre;
    string correo;
    string carrera;
    int anio;

    Nodo* siguiente;
    Nodo* anterior;

    Nodo(int _id,
          string _nombre,
          string _correo,
          string _carrera,
          int _anio) {

        id = _id;
        nombre = _nombre;
        correo = _correo;
        carrera = _carrera;
        anio = _anio;

        siguiente = nullptr;
        anterior = nullptr;
    }
};

// ============================
// CLASE LISTA DOBLE
// ============================

class ListaDoble {

private:

    Nodo* cabeza;
    int idActual;

public:

    // Constructor
    ListaDoble() {

        cabeza = nullptr;
        idActual = 1;

        cargarDesdeArchivo();
    }

    // ============================
    // AGREGAR ESTUDIANTE
    // ============================

    void agregar(string nombre,
                 string correo,
                 string carrera,
                 int anio) {

        Nodo* nuevo = new Nodo(
            idActual++,
            nombre,
            correo,
            carrera,
            anio
        );

        if (cabeza == nullptr) {

            cabeza = nuevo;

        } else {

            Nodo* actual = cabeza;

            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }

            actual->siguiente = nuevo;
            nuevo->anterior = actual;
        }

        guardarEnArchivo();

        cout << "\nEstudiante agregado correctamente.\n";
    }

    // ============================
    // MOSTRAR REGISTROS
    // ============================

    void mostrar() {

        if (cabeza == nullptr) {

            cout << "\nNo existen registros.\n";
            return;
        }

        Nodo* actual = cabeza;

        cout << "\n===============================================================\n";
        cout << "ID\tNombre\t\tCorreo\t\t\tCarrera\t\tAño\n";
        cout << "===============================================================\n";

        while (actual != nullptr) {

            cout << actual->id << "\t"
                 << actual->nombre << "\t\t"
                 << actual->correo << "\t"
                 << actual->carrera << "\t"
                 << actual->anio << endl;

            actual = actual->siguiente;
        }

        cout << "===============================================================\n";
    }

    // ============================
    // BUSCAR POR NOMBRE
    // ============================

    void buscarPorNombre(string nombreBuscar) {

        Nodo* actual = cabeza;

        bool encontrado = false;

        while (actual != nullptr) {

            if (actual->nombre == nombreBuscar) {

                cout << "\n===== REGISTRO ENCONTRADO =====\n";

                cout << "ID: " << actual->id << endl;
                cout << "Nombre: " << actual->nombre << endl;
                cout << "Correo: " << actual->correo << endl;
                cout << "Carrera: " << actual->carrera << endl;
                cout << "Anio: " << actual->anio << endl;

                encontrado = true;

                break;
            }

            actual = actual->siguiente;
        }

        if (!encontrado) {

            cout << "\nNo se encontró el estudiante.\n";
        }
    }

    // ============================
    // BUSCAR POR ID
    // ============================

    void buscarPorID(int idBuscar) {

        Nodo* actual = cabeza;

        while (actual != nullptr) {

            if (actual->id == idBuscar) {

                cout << "\n===== REGISTRO ENCONTRADO =====\n";

                cout << "ID: " << actual->id << endl;
                cout << "Nombre: " << actual->nombre << endl;
                cout << "Correo: " << actual->correo << endl;
                cout << "Carrera: " << actual->carrera << endl;
                cout << "Anio: " << actual->anio << endl;

                return;
            }

            actual = actual->siguiente;
        }

        cout << "\nID no encontrado.\n";
    }

    // ============================
    // MODIFICAR ESTUDIANTE
    // ============================

    void modificar(int idModificar) {

        Nodo* actual = cabeza;

        while (actual != nullptr) {

            if (actual->id == idModificar) {

                cout << "\n===== MODIFICAR ESTUDIANTE =====\n";

                cout << "Nuevo nombre: ";
                getline(cin, actual->nombre);

                cout << "Nuevo correo: ";
                getline(cin, actual->correo);

                cout << "Nueva carrera: ";
                getline(cin, actual->carrera);

                cout << "Nuevo año: ";
                cin >> actual->anio;
                cin.ignore();

                guardarEnArchivo();

                cout << "\nRegistro actualizado correctamente.\n";

                return;
            }

            actual = actual->siguiente;
        }

        cout << "\nID no encontrado.\n";
    }

    // ============================
    // ELIMINAR ESTUDIANTE
    // ============================

    void eliminar(int idEliminar) {

        Nodo* actual = cabeza;

        while (actual != nullptr) {

            if (actual->id == idEliminar) {

                // Si no es el primero
                if (actual->anterior != nullptr) {

                    actual->anterior->siguiente = actual->siguiente;

                } else {

                    cabeza = actual->siguiente;
                }

                // Si no es el último
                if (actual->siguiente != nullptr) {

                    actual->siguiente->anterior = actual->anterior;
                }

                delete actual;

                guardarEnArchivo();

                cout << "\nRegistro eliminado correctamente.\n";

                return;
            }

            actual = actual->siguiente;
        }

        cout << "\nID no encontrado.\n";
    }

    // ============================
    // GUARDAR EN ARCHIVO
    // ============================

    void guardarEnArchivo() {

        ofstream archivo("registros.txt");

        Nodo* actual = cabeza;

        while (actual != nullptr) {

            archivo
                << actual->id << ";"
                << actual->nombre << ";"
                << actual->correo << ";"
                << actual->carrera << ";"
                << actual->anio
                << endl;

            actual = actual->siguiente;
        }

        archivo.close();
    }

    // ============================
    // CARGAR DESDE ARCHIVO
    // ============================

    void cargarDesdeArchivo() {

        ifstream archivo("registros.txt");

        string linea;

        while (getline(archivo, linea)) {

            stringstream ss(linea);

            string campo;

            int id;
            string nombre;
            string correo;
            string carrera;
            int anio;

            getline(ss, campo, ';');
            id = stoi(campo);

            getline(ss, nombre, ';');
            getline(ss, correo, ';');
            getline(ss, carrera, ';');

            getline(ss, campo, ';');
            anio = stoi(campo);

            Nodo* nuevo = new Nodo(
                id,
                nombre,
                correo,
                carrera,
                anio
            );

            if (cabeza == nullptr) {

                cabeza = nuevo;

            } else {

                Nodo* actual = cabeza;

                while (actual->siguiente != nullptr) {
                    actual = actual->siguiente;
                }

                actual->siguiente = nuevo;
                nuevo->anterior = actual;
            }

            if (id >= idActual) {
                idActual = id + 1;
            }
        }

        archivo.close();
    }

    // ============================
    // RECARGAR ARCHIVO
    // ============================

    void recargarDesdeArchivo() {

        Nodo* actual = cabeza;

        while (actual != nullptr) {

            Nodo* temp = actual;

            actual = actual->siguiente;

            delete temp;
        }

        cabeza = nullptr;

        idActual = 1;

        cargarDesdeArchivo();

        cout << "\nRegistros cargados nuevamente.\n";
    }
};

// ============================
// FUNCIÓN PRINCIPAL
// ============================

int main() {

    ListaDoble lista;

    int opcion;

    do {

        cout << "\n";
        cout << "=========================================\n";
        cout << " SISTEMA DE REGISTRO ACADEMICO\n";
        cout << "=========================================\n";

        cout << "1. Agregar estudiante\n";
        cout << "2. Mostrar registros\n";
        cout << "3. Buscar por nombre\n";
        cout << "4. Buscar por ID\n";
        cout << "5. Modificar estudiante\n";
        cout << "6. Eliminar estudiante\n";
        cout << "7. Recargar archivo\n";
        cout << "8. Salir\n";

        cout << "Seleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        // ============================
        // OPCIÓN 1
        // ============================

        if (opcion == 1) {

            string nombre;
            string correo;
            string carrera;
            int anio;

            cout << "\nNombre completo: ";
            getline(cin, nombre);

            cout << "Correo: ";
            getline(cin, correo);

            cout << "Carrera profesional: ";
            getline(cin, carrera);

            cout << "Anio de ingreso: ";
            cin >> anio;
            cin.ignore();

            lista.agregar(
                nombre,
                correo,
                carrera,
                anio
            );
        }

        // ============================
        // OPCIÓN 2
        // ============================

        else if (opcion == 2) {

            lista.mostrar();
        }

        // ============================
        // OPCIÓN 3
        // ============================

        else if (opcion == 3) {

            string nombreBuscar;

            cout << "\nIngrese nombre: ";

            getline(cin, nombreBuscar);

            lista.buscarPorNombre(nombreBuscar);
        }

        // ============================
        // OPCIÓN 4
        // ============================

        else if (opcion == 4) {

            int id;

            cout << "\nIngrese ID: ";

            cin >> id;
            cin.ignore();

            lista.buscarPorID(id);
        }

        // ============================
        // OPCIÓN 5
        // ============================

        else if (opcion == 5) {

            int id;

            cout << "\nIngrese ID a modificar: ";

            cin >> id;
            cin.ignore();

            lista.modificar(id);
        }

        // ============================
        // OPCIÓN 6
        // ============================

        else if (opcion == 6) {

            int id;

            cout << "\nIngrese ID a eliminar: ";

            cin >> id;
            cin.ignore();

            lista.eliminar(id);
        }

        // ============================
        // OPCIÓN 7
        // ============================

        else if (opcion == 7) {

            lista.recargarDesdeArchivo();
        }

        // ============================
        // OPCIÓN 8
        // ============================

        else if (opcion == 8) {

            cout << "\nGracias por usar el sistema.\n";
        }

        // ============================
        // OPCIÓN INVÁLIDA
        // ============================

        else {

            cout << "\nOpción inválida.\n";
        }

    } while (opcion != 8);

    return 0;
}