#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//----------------------------
// Estructura
//----------------------------
struct Estudiante {
    string nombre;
    string correo;
    string carrera;
    int anio;
};

vector<Estudiante> lista;

// Controles globales
HWND hNombre, hCorreo, hCarrera, hAnio, hLista;
HINSTANCE g_hInst;

//----------------------------
// Agregar estudiante
//----------------------------
void AgregarEstudiante() {
    char nombre[100];
    char correo[100];
    char carrera[100];
    char anioStr[20];

    GetWindowTextA(hNombre, nombre, 100);
    GetWindowTextA(hCorreo, correo, 100);
    GetWindowTextA(hCarrera, carrera, 100);
    GetWindowTextA(hAnio, anioStr, 20);

    int anio = atoi(anioStr);

    Estudiante e = { nombre, correo, carrera, anio };
    lista.push_back(e);

    string texto = string(nombre) + " - " + correo;

    SendMessageA(
        hLista,
        LB_ADDSTRING,
        0,
        (LPARAM)texto.c_str()
    );

    // limpiar cajas
    SetWindowTextA(hNombre, "");
    SetWindowTextA(hCorreo, "");
    SetWindowTextA(hCarrera, "");
    SetWindowTextA(hAnio, "");
}

//----------------------------
// Guardar archivo
//----------------------------
void GuardarArchivo() {
    ofstream file("registros.txt");

    for (auto& e : lista) {
        file << e.nombre << ";"
             << e.correo << ";"
             << e.carrera << ";"
             << e.anio << endl;
    }

    file.close();
}

//----------------------------
// Cargar archivo
//----------------------------
void CargarArchivo() {
    lista.clear();

    SendMessage(hLista, LB_RESETCONTENT, 0, 0);

    ifstream file("registros.txt");
    string linea;

    while (getline(file, linea)) {
        stringstream ss(linea);

        string nombre, correo, carrera, anioStr;

        getline(ss, nombre, ';');
        getline(ss, correo, ';');
        getline(ss, carrera, ';');
        getline(ss, anioStr, ';');

        int anio = stoi(anioStr);

        Estudiante e = { nombre, correo, carrera, anio };
        lista.push_back(e);

        string texto = nombre + " - " + correo;

        SendMessageA(
            hLista,
            LB_ADDSTRING,
            0,
            (LPARAM)texto.c_str()
        );
    }

    file.close();
}

//----------------------------
// Crear controles
//----------------------------
void CrearControles(HWND hwnd) {

    CreateWindowA("STATIC", "Nombre:",
        WS_VISIBLE | WS_CHILD,
        20, 20, 80, 20,
        hwnd, NULL, g_hInst, NULL);

    hNombre = CreateWindowA("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 20, 250, 25,
        hwnd, NULL, g_hInst, NULL);


    CreateWindowA("STATIC", "Correo:",
        WS_VISIBLE | WS_CHILD,
        20, 60, 80, 20,
        hwnd, NULL, g_hInst, NULL);

    hCorreo = CreateWindowA("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 60, 250, 25,
        hwnd, NULL, g_hInst, NULL);


    CreateWindowA("STATIC", "Carrera:",
        WS_VISIBLE | WS_CHILD,
        20, 100, 80, 20,
        hwnd, NULL, g_hInst, NULL);

    hCarrera = CreateWindowA("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 100, 250, 25,
        hwnd, NULL, g_hInst, NULL);


    CreateWindowA("STATIC", "Año:",
        WS_VISIBLE | WS_CHILD,
        20, 140, 80, 20,
        hwnd, NULL, g_hInst, NULL);

    hAnio = CreateWindowA("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 140, 100, 25,
        hwnd, NULL, g_hInst, NULL);


    // Botones
    CreateWindowA("BUTTON", "Agregar",
        WS_VISIBLE | WS_CHILD,
        380, 20, 100, 30,
        hwnd, (HMENU)1, g_hInst, NULL);

    CreateWindowA("BUTTON", "Guardar",
        WS_VISIBLE | WS_CHILD,
        380, 70, 100, 30,
        hwnd, (HMENU)2, g_hInst, NULL);

    CreateWindowA("BUTTON", "Cargar",
        WS_VISIBLE | WS_CHILD,
        380, 120, 100, 30,
        hwnd, (HMENU)3, g_hInst, NULL);


    // Lista más grande
    hLista = CreateWindowA("LISTBOX", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
        20, 200, 460, 200,
        hwnd, NULL, g_hInst, NULL);
}

//----------------------------
// Procesar eventos
//----------------------------
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 1:
            AgregarEstudiante();
            break;

        case 2:
            GuardarArchivo();
            break;

        case 3:
            CargarArchivo();
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

//----------------------------
// WinMain
//----------------------------
int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrev,
    LPSTR args,
    int nCmdShow
) {
    g_hInst = hInst;

    WNDCLASSA wc = { 0 };

    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.lpszClassName = "RegistroWin";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc)) {
        return -1;
    }

    HWND hwnd = CreateWindowA(
        "RegistroWin",
        "Sistema de Registro Academico",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100,
        100,
        550,   // ancho corregido
        500,   // alto corregido
        NULL,
        NULL,
        hInst,
        NULL
    );

    CrearControles(hwnd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}