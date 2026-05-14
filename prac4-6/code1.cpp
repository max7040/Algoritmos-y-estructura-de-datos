#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//--------------------------------------
// CLASE NODO (LISTA DOBLE)
//--------------------------------------
class Nodo {
public:
    int id;
    string nombre, correo, carrera;
    int anio;

    Nodo* siguiente;
    Nodo* anterior;

    Nodo(int _id, string _nombre, string _correo,
         string _carrera, int _anio) {

        id = _id;
        nombre = _nombre;
        correo = _correo;
        carrera = _carrera;
        anio = _anio;

        siguiente = nullptr;
        anterior = nullptr;
    }
};

//--------------------------------------
// LISTA DOBLEMENTE ENLAZADA
//--------------------------------------
class ListaDoble {
private:
    Nodo* cabeza;
    int idActual;

public:
    ListaDoble() {
        cabeza = nullptr;
        idActual = 1;
        cargarArchivo();
    }

    void agregar(string nombre, string correo, string carrera, int anio) {
        Nodo* nuevo = new Nodo(idActual++, nombre, correo, carrera, anio);

        if (cabeza == nullptr) {
            cabeza = nuevo;
        }
        else {
            Nodo* actual = cabeza;

            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }

            actual->siguiente = nuevo;
            nuevo->anterior = actual;
        }

        guardarArchivo();
    }

    void guardarArchivo() {
        ofstream file("registros.txt");

        Nodo* actual = cabeza;

        while (actual != nullptr) {
            file << actual->id << ";"
                 << actual->nombre << ";"
                 << actual->correo << ";"
                 << actual->carrera << ";"
                 << actual->anio << endl;

            actual = actual->siguiente;
        }

        file.close();
    }

    void cargarArchivo() {
        ifstream file("registros.txt");
        string linea;

        while (getline(file, linea)) {
            stringstream ss(linea);

            string campo;
            string nombre, correo, carrera;
            int id, anio;

            getline(ss, campo, ';');
            id = stoi(campo);

            getline(ss, nombre, ';');
            getline(ss, correo, ';');
            getline(ss, carrera, ';');

            getline(ss, campo, ';');
            anio = stoi(campo);

            Nodo* nuevo = new Nodo(id, nombre, correo, carrera, anio);

            if (cabeza == nullptr) {
                cabeza = nuevo;
            }
            else {
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

        file.close();
    }

    void mostrarEnListBox(HWND hLista) {
        SendMessage(hLista, LB_RESETCONTENT, 0, 0);

        Nodo* actual = cabeza;

        while (actual != nullptr) {
            string texto =
                to_string(actual->id) + " | " +
                actual->nombre + " | " +
                actual->correo + " | " +
                actual->carrera + " | " +
                to_string(actual->anio);

            SendMessage(
                hLista,
                LB_ADDSTRING,
                0,
                (LPARAM)texto.c_str()
            );

            actual = actual->siguiente;
        }
    }

    void eliminar(int idEliminar) {
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            if (actual->id == idEliminar) {

                if (actual->anterior != nullptr) {
                    actual->anterior->siguiente = actual->siguiente;
                }
                else {
                    cabeza = actual->siguiente;
                }

                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }

                delete actual;
                guardarArchivo();
                return;
            }

            actual = actual->siguiente;
        }
    }
};

//--------------------------------------
// OBJETO GLOBAL
//--------------------------------------
ListaDoble lista;

//--------------------------------------
// CONTROLES
//--------------------------------------
HWND hNombre;
HWND hCorreo;
HWND hCarrera;
HWND hAnio;
HWND hEliminarID;
HWND hLista;

//--------------------------------------
// AGREGAR
//--------------------------------------
void agregarEstudiante() {
    char nombre[100];
    char correo[100];
    char carrera[100];
    char anioStr[20];

    GetWindowText(hNombre, nombre, 100);
    GetWindowText(hCorreo, correo, 100);
    GetWindowText(hCarrera, carrera, 100);
    GetWindowText(hAnio, anioStr, 20);

    int anio = atoi(anioStr);

    lista.agregar(nombre, correo, carrera, anio);
    lista.mostrarEnListBox(hLista);

    SetWindowText(hNombre, "");
    SetWindowText(hCorreo, "");
    SetWindowText(hCarrera, "");
    SetWindowText(hAnio, "");
}

//--------------------------------------
// ELIMINAR
//--------------------------------------
void eliminarEstudiante() {
    char idStr[20];

    GetWindowText(hEliminarID, idStr, 20);

    int id = atoi(idStr);

    lista.eliminar(id);
    lista.mostrarEnListBox(hLista);

    SetWindowText(hEliminarID, "");
}

//--------------------------------------
// PROCEDIMIENTO VENTANA
//--------------------------------------
LRESULT CALLBACK WindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
) {
    switch (msg) {

    case WM_COMMAND:

        switch (LOWORD(wp)) {

        case 1:
            agregarEstudiante();
            break;

        case 2:
            eliminarEstudiante();
            break;

        case 3:
            lista.mostrarEnListBox(hLista);
            break;
        }

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    return 0;
}

//--------------------------------------
// CREAR CONTROLES
//--------------------------------------
void crearControles(HWND hwnd) {

    CreateWindow(
        "STATIC",
        "Nombre:",
        WS_VISIBLE | WS_CHILD,
        20, 20, 80, 20,
        hwnd, NULL, NULL, NULL
    );

    hNombre = CreateWindow(
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 20, 200, 20,
        hwnd, NULL, NULL, NULL
    );

    CreateWindow(
        "STATIC",
        "Correo:",
        WS_VISIBLE | WS_CHILD,
        20, 50, 80, 20,
        hwnd, NULL, NULL, NULL
    );

    hCorreo = CreateWindow(
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 50, 200, 20,
        hwnd, NULL, NULL, NULL
    );

    CreateWindow(
        "STATIC",
        "Carrera:",
        WS_VISIBLE | WS_CHILD,
        20, 80, 80, 20,
        hwnd, NULL, NULL, NULL
    );

    hCarrera = CreateWindow(
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 80, 200, 20,
        hwnd, NULL, NULL, NULL
    );

    CreateWindow(
        "STATIC",
        "Año:",
        WS_VISIBLE | WS_CHILD,
        20, 110, 80, 20,
        hwnd, NULL, NULL, NULL
    );

    hAnio = CreateWindow(
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 110, 100, 20,
        hwnd, NULL, NULL, NULL
    );

    CreateWindow(
        "BUTTON",
        "Agregar",
        WS_VISIBLE | WS_CHILD,
        320, 20, 100, 30,
        hwnd, (HMENU)1, NULL, NULL
    );

    CreateWindow(
        "BUTTON",
        "Mostrar",
        WS_VISIBLE | WS_CHILD,
        320, 60, 100, 30,
        hwnd, (HMENU)3, NULL, NULL
    );

    CreateWindow(
        "STATIC",
        "ID Eliminar:",
        WS_VISIBLE | WS_CHILD,
        20, 150, 80, 20,
        hwnd, NULL, NULL, NULL
    );

    hEliminarID = CreateWindow(
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 150, 100, 20,
        hwnd, NULL, NULL, NULL
    );

    CreateWindow(
        "BUTTON",
        "Eliminar",
        WS_VISIBLE | WS_CHILD,
        320, 100, 100, 30,
        hwnd, (HMENU)2, NULL, NULL
    );

    hLista = CreateWindow(
        "LISTBOX",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
        20, 200, 500, 220,
        hwnd, NULL, NULL, NULL
    );
}

//--------------------------------------
// MAIN WINAPI
//--------------------------------------
int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrev,
    LPSTR args,
    int nCmdShow
) {
    WNDCLASS wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "RegistroAcademico";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) {
        return -1;
    }

    HWND hwnd = CreateWindow(
        "RegistroAcademico",
        "Sistema de Registro Academico - Lista Doble",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100,
        100,
        600,
        500,
        NULL,
        NULL,
        NULL,
        NULL
    );

    crearControles(hwnd);

    lista.mostrarEnListBox(hLista);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}