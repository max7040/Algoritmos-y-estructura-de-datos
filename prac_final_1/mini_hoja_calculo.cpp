#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

struct Estudiante {
    string codigo;
    string nombre;
    float nota1;
    float nota2;
    float nota3;
    float promedio;
};

// Estructuras de almacenamiento global
vector<Estudiante> hoja;
unordered_map<string, int> indiceHash;
stack<vector<Estudiante>> historial;
queue<string> colaAtencion;

// Función en línea para cálculo aritmético eficiente
inline float calcularPromedio(float n1, float n2, float n3) {
    return (n1 + n2 + n3) / 3.0f;
}

// Reconstrucción del índice asociativo O(n)
void actualizarHash() {
    indiceHash.clear();

    for (size_t i = 0; i < hoja.size(); ++i) {
        indiceHash[hoja[i].codigo] = static_cast<int>(i);
    }
}

// Resguardo del estado actual previo a mutaciones
void guardarHistorial() {
    historial.push(hoja);
}

void registrarEstudiante() {
    Estudiante e;

    cout << "\n--- REGISTRAR NUEVO ESTUDIANTE --"<<endl;
    cout << "Codigo unico: "<<endl;
    cin >> e.codigo;

    // Validación de clave primaria duplicada en tiempo O(1)
    if (indiceHash.find(e.codigo) != indiceHash.end()) {
        cout << "\n[ERROR] El codigo ingresado ya se encuentra registrado."<<endl;
        return;
    }
        cin.ignore();

    cout << "\nApellidos y Nombres: "<<endl;
    getline(cin, e.nombre);

    cout << "\nNota 1: "<<endl;
    cin >> e.nota1;

    cout << "\nNota 2: "<<endl;
    cin >> e.nota2;

    cout << "\nNota 3: "<<endl;
    cin >> e.nota3;

    e.promedio = calcularPromedio(e.nota1, e.nota2, e.nota3);

    guardarHistorial();
    hoja.push_back(e);
    actualizarHash();

    cout << "\n[EXITO] Registro insertado e indexado correctamente."<<endl;
}

void mostrarHoja() {
    if (hoja.empty()) {
        cout << "\n[AVISO] La hoja de calculo no contiene registros."<<endl;
        return;
    }

    cout << "\n====================================================================";
    cout << "\n                   MINI HOJA DE CALCULO ACADEMICA                   ";
    cout << "\n====================================================================";

    cout << left << setw(12) << "\nCodigo"
         << setw(25) << "Nombre Completo"
         << setw(8)  << "N1"
         << setw(8)  << "N2"
         << setw(8)  << "N3"
         << setw(10) << "Promedio"
         << endl;

    cout << "-------------------------------------------------------------------"<<endl;

    for (const auto &e : hoja) {
        cout << left << setw(12) << e.codigo
             << setw(25) << e.nombre
             << setw(8)  << setprecision(1) << fixed << e.nota1
             << setw(8)  << e.nota2
             << setw(8)  << e.nota3
             << setw(10) << setprecision(2) << e.promedio<< endl;
    }

    cout << "\n===================================================================="<<endl;
}

void buscarEstudiante() {
    string codigo;

    cout << "\nIngrese el codigo a buscar en Tabla Hash: "<<endl;
    cin >> codigo;

    auto it = indiceHash.find(codigo);

    if (it != indiceHash.end()) {
        int pos = it->second;
        const auto &e = hoja[pos];

        cout << "\n[REGISTRO ENCONTRADO - BUSQUEDA O(1)] "<<endl;
        cout << "\nEstudiante: "
             << e.nombre
             << " | Promedio Actual: "
             << fixed 
             <<setprecision(2)
             << e.promedio
             << endl;
    } else {
        cout << "\n[AVISO] Codigo no registrado en la base indexada."<<endl;
    }
}

// QuickSort: Particionamiento por Promedio (Descendente de Mayor a Menor)
int particion(vector<Estudiante> &arr, int bajo, int alto) {
    float pivote = arr[alto].promedio;
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (arr[j].promedio >= pivote) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[alto]);
    return i + 1;
}

void quickSort(vector<Estudiante> &arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(arr, bajo, alto);

        quickSort(arr, bajo, pi - 1);   
        quickSort(arr, pi + 1, alto);
    }
}

// MergeSort: Combinación por Código Alfanumérico (Ascendente)
void merge(vector<Estudiante> &arr, int izq, int medio, int der) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;

    vector<Estudiante> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[izq + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = izq;

    while (i < n1 && j < n2) {
        if (L[i].codigo <= R[j].codigo) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Estudiante> &arr, int izq, int der) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;

        mergeSort(arr, izq, medio);
        mergeSort(arr, medio + 1, der);
        merge(arr, izq, medio, der);
    }
}

void ordenarPorPromedio() {
    if (hoja.empty())
        return;

    guardarHistorial();
        quickSort(hoja, 0, static_cast<int>(hoja.size() - 1));
    actualizarHash();

    cout << "[OK] Filas ordenadas por Promedio (Descendente) via QuickSort."<<endl;
}

void ordenarPorCodigo() {
    if (hoja.empty())
        return;

    guardarHistorial();

    mergeSort(hoja, 0, static_cast<int>(hoja.size() - 1));
    actualizarHash();

    cout << "[OK] Filas ordenadas por Codigo (Ascendente) via MergeSort."<<endl;
}

void deshacer() {
    if (!historial.empty()) {
        hoja = historial.top();
        historial.pop();

        actualizarHash();

        cout << "[UNDO] Estado de celdas restaurado al punto de control anterior."<<endl;
    } else {
        cout << "[INFO] No existen acciones registradas para deshacer."<<endl;
    }
}

void agregarColaAtencion() {
    string codigo;

    cout << "Ingrese el codigo del estudiante que entra a la cola: "<<endl;
    cin >> codigo;

    if (indiceHash.find(codigo) != indiceHash.end()) {
        colaAtencion.push(codigo);

        cout << "[COLA] Estudiante ingresado en la cola FIFO de secretaria."<<endl;
    } else {
        cout << "[ERROR] El codigo no figura registrado en la hoja actual."<<endl;
    }
}

void atenderEstudiante() {
    if (!colaAtencion.empty()) {
        string codigo = colaAtencion.front();
        colaAtencion.pop();

        int pos = indiceHash[codigo];

        cout << "[ATENCION] Despachando requerimiento de: "
             << hoja[pos].nombre
             << endl;
    } else {
        cout << "[INFO] Canal de atencion despejado. Cola vacia."<<endl;
    }
}

// Funcion Recursiva Pura para la acumulacion de promedios O(n)
float sumaPromediosRecursiva(size_t i) {
    if (i == hoja.size())
        return 0.0f;

    return hoja[i].promedio + sumaPromediosRecursiva(i + 1);
}

void estadisticas() {
    if (hoja.empty()) {
        cout << "[AVISO] Hoja sin datos para el calculo estadistico."<<endl;
        return;
    }
        float suma = sumaPromediosRecursiva(0);
    float promedioGeneral = suma / static_cast<float>(hoja.size());

    cout << "========= METRICAS ANALITICAS GENERALES ========="<<endl;
    cout << " Estudiantes Procesados : " << hoja.size()<< endl;
    cout << " Promedio General Coho. : "
         << fixed
         << setprecision(2)
         << promedioGeneral<< endl;
    cout << "================================================="<<endl;
}

void menu() {
    int opcion;

    do {
        cout << "============================================="<< endl;
        cout << "         MENU PRINCIPAL - SPREADSHEET        "<< endl;
        cout << "============================================="<< endl;
        cout << "1. Registrar Estudiante (Fila) "<< endl;
        cout << "2. Mostrar Hoja de Calculo "<< endl;
        cout << "3. Buscar Estudiante por Codigo (Hash) "<< endl;
        cout << "4. Ordenar por Promedio [QuickSort Desc] "<< endl;
        cout << "5. Ordenar por Codigo   [MergeSort Asc] "<< endl;
        cout << "6. Deshacer Ultima Modificacion [Stack] "<< endl;
        cout << "7. Insertar Estudiante a Cola de Atencion "<< endl;
        cout << "8. Atender Siguiente Estudiante [FIFO] "<< endl;
        cout << "9. Mostrar Estadisticas Consolidadas "<< endl;
        cout << "10. Salir de la Aplicacion "<< endl;
        cout << "--------------------------------------------"<< endl;
        cout << "Seleccione una opcion (1-10): "<< endl;

        if (!(cin >> opcion)) {
            cout << "[ALERTA] Entrada invalida.";

            cin.clear();
            cin.ignore(10000, ' ');

            continue;
        }

        switch (opcion) {
            case 1:
                registrarEstudiante();
                break;

            case 2:
                mostrarHoja();
                break;

            case 3:
                buscarEstudiante();
                break;

            case 4:
                ordenarPorPromedio();
                break;

            case 5:
                ordenarPorCodigo();
                break;

            case 6:
                deshacer();
                break;

            case 7:
                agregarColaAtencion();
                break;

            case 8:
                atenderEstudiante();
                break;

            case 9:
                estadisticas();
                break;
                            case 10:
                cout << "Liberando memoria interna... Programa cerrado.";
                break;

            default:
                cout << "[ERROR] Opcion fuera de rango.";
        }

    } while (opcion != 10);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    menu();

    return 0;
}