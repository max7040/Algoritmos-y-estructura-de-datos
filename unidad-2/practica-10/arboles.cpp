/*
 * ============================================================
 *  PRÁCTICA N.° 09 — Árboles Binarios de Búsqueda (BST)
 *  Algoritmos y Estructuras de Datos — SIS210 — UNAP Puno
 * ------------------------------------------------------------
 *  Implementación 100% manual:
 *    - Sin unique_ptr (punteros crudos + delete explícito)
 *    - Sin STL para la lógica del árbol (ni map, ni set)
 *    - Sí usamos vector y queue de STL SOLO para devolver
 *      resultados al main(), porque la práctica los pide;
 *      pero la estructura interna del BST es toda manual.
 *    - Cada operación tiene comentarios paso a paso.
 *
 *  Compilar:
 *    g++ -std=c++17 -O2 -Wall -o bst bst_academico.cpp && ./bst
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <random>
#include <algorithm>
#include <cassert>
#include <climits>

// ════════════════════════════════════════════════════════════
// 1.  ENUMERADO — Estado académico del estudiante
// ════════════════════════════════════════════════════════════
enum EstadoAcademico {
    ACTIVO,
    EGRESADO,
    RETIRADO,
    SUSPENDIDO
};

// Convierte el enum a texto para imprimir
std::string estadoStr(EstadoAcademico e) {
    if (e == ACTIVO)     return "ACTIVO";
    if (e == EGRESADO)   return "EGRESADO";
    if (e == RETIRADO)   return "RETIRADO";
    if (e == SUSPENDIDO) return "SUSPENDIDO";
    return "DESCONOCIDO";
}

// ════════════════════════════════════════════════════════════
// 2.  STRUCT Estudiante — expediente académico (el "dato")
// ════════════════════════════════════════════════════════════
struct Estudiante {
    int              codigo;           // clave primaria del BST (8 dígitos)
    std::string      nombre;
    std::string      escuela;
    float            ppa;              // Promedio Ponderado Acumulado [0, 20]
    int              creditos;
    EstadoAcademico  estado;
    std::string      semestre_ingreso;

    // Constructor con validación (equivale a __post_init__ de Python)
    Estudiante(int cod, std::string nom, std::string esc,
               float pp, int cred, EstadoAcademico est, std::string sem)
        : codigo(cod), nombre(nom), escuela(esc),
          ppa(pp), creditos(cred), estado(est), semestre_ingreso(sem)
    {
        // Validar rango del código
        if (cod < 10000000 || cod > 29999999) {
            std::cout << "ERROR: Codigo invalido: " << cod << "\n";
            // En la práctica real podrías lanzar una excepción aquí
        }
        // Validar rango del PPA
        if (pp < 0.0f || pp > 20.0f) {
            std::cout << "ERROR: PPA fuera de rango [0,20]: " << pp << "\n";
        }
    }

    // Constructor vacío (necesario para asignaciones internas)
    Estudiante() : codigo(0), ppa(0.0f), creditos(0), estado(ACTIVO) {}

    void imprimir() const {
        std::cout << std::left
                  << std::setw(12) << codigo
                  << std::setw(35) << nombre
                  << std::setw(20) << escuela
                  << "PPA:" << std::fixed << std::setprecision(1) << ppa
                  << "  " << estadoStr(estado) << "\n";
    }
};

// ════════════════════════════════════════════════════════════
// 3.  STRUCT NodoBST — bloque básico del árbol
// ════════════════════════════════════════════════════════════
/*
 *  Cada nodo tiene:
 *    dato       → el expediente del estudiante
 *    izquierdo  → puntero al hijo con código MENOR
 *    derecho    → puntero al hijo con código MAYOR
 *
 *  Cuando un hijo no existe, el puntero vale nullptr (= "vacío").
 *
 *  En memoria real se ve así:
 *
 *       [ dato | *izq | *der ]
 *           |       |       |
 *       Estudiante  |       +----→ NodoBST (código mayor)
 *                   +--------→ NodoBST (código menor)
 */
struct NodoBST {
    Estudiante dato;
    NodoBST*   izquierdo;   // hijo izquierdo (código menor)
    NodoBST*   derecho;     // hijo derecho   (código mayor)

    // Al crear un nodo, sus hijos parten como nullptr (es una hoja)
    NodoBST(Estudiante e) : dato(e), izquierdo(nullptr), derecho(nullptr) {}
};

// ════════════════════════════════════════════════════════════
// 4.  CLASE ArbolAcademico — el BST completo
// ════════════════════════════════════════════════════════════
class ArbolAcademico {
private:
    NodoBST* raiz;   // único punto de entrada al árbol

    // ----------------------------------------------------------
    // AUXILIAR: liberar toda la memoria del árbol (post-order)
    // Se llama en el destructor. Post-order porque primero
    // liberamos los hijos y luego el padre (si fuera al revés,
    // perderíamos los punteros a los hijos).
    // ----------------------------------------------------------
    void liberarMemoria(NodoBST* nodo) {
        if (nodo == nullptr) return;          // árbol vacío o llegamos a una hoja
        liberarMemoria(nodo->izquierdo);      // 1. liberar subárbol izquierdo
        liberarMemoria(nodo->derecho);        // 2. liberar subárbol derecho
        delete nodo;                          // 3. liberar este nodo
    }

    // ----------------------------------------------------------
    // INSERTAR (versión recursiva interna)
    // Retorna el puntero al nodo raíz del subárbol resultante.
    // ----------------------------------------------------------
    NodoBST* insertar(NodoBST* nodo, Estudiante e) {
        // CASO BASE: llegamos a una posición vacía → crear nuevo nodo aquí
        if (nodo == nullptr) {
            return new NodoBST(e);   // reservar memoria para el nuevo nodo
        }

        // PASO 1: comparar el código del nuevo estudiante con el nodo actual
        if (e.codigo < nodo->dato.codigo) {
            // El código es MENOR → va al subárbol IZQUIERDO
            // Llamada recursiva: insertamos en el hijo izquierdo y
            // actualizamos el puntero (por si era nullptr y ahora es un nodo)
            nodo->izquierdo = insertar(nodo->izquierdo, e);

        } else if (e.codigo > nodo->dato.codigo) {
            // El código es MAYOR → va al subárbol DERECHO
            nodo->derecho = insertar(nodo->derecho, e);

        } else {
            // El código es IGUAL → duplicado, no se permite
            std::cout << "ADVERTENCIA: Codigo duplicado ignorado: "
                      << e.codigo << "\n";
        }

        // Retornamos el puntero al nodo actual (sin cambios en este nivel)
        return nodo;
    }

    // ----------------------------------------------------------
    // BUSCAR (versión recursiva interna)
    // Retorna puntero al nodo encontrado, o nullptr si no existe.
    // ----------------------------------------------------------
    NodoBST* buscar(NodoBST* nodo, int codigo) {
        // CASO BASE 1: llegamos a nullptr → el código no existe
        if (nodo == nullptr) return nullptr;

        // CASO BASE 2: encontramos el código
        if (nodo->dato.codigo == codigo) return nodo;

        // PASO 1: ¿el código buscado es menor que el nodo actual?
        if (codigo < nodo->dato.codigo) {
            // Buscar en el subárbol IZQUIERDO (códigos menores)
            return buscar(nodo->izquierdo, codigo);
        } else {
            // Buscar en el subárbol DERECHO (códigos mayores)
            return buscar(nodo->derecho, codigo);
        }
    }

    // ----------------------------------------------------------
    // MÍNIMO: nodo con el código más pequeño en un subárbol.
    // Siempre es el nodo más a la IZQUIERDA posible.
    // Se usa en la eliminación (Caso 3: dos hijos).
    // ----------------------------------------------------------
    NodoBST* minimo(NodoBST* nodo) {
        // Bajar siempre a la izquierda hasta que no haya más
        while (nodo->izquierdo != nullptr) {
            nodo = nodo->izquierdo;
        }
        return nodo;   // el nodo más izquierdo es el mínimo
    }

    // ----------------------------------------------------------
    // MÁXIMO: nodo con el código más grande en un subárbol.
    // Siempre es el nodo más a la DERECHA posible.
    // ----------------------------------------------------------
    NodoBST* maximo(NodoBST* nodo) {
        while (nodo->derecho != nullptr) {
            nodo = nodo->derecho;
        }
        return nodo;
    }

    // ----------------------------------------------------------
    // ELIMINAR (versión recursiva interna)
    // Retorna el nuevo puntero raíz del subárbol tras la eliminación.
    // Tiene TRES casos según cuántos hijos tenga el nodo a eliminar.
    // ----------------------------------------------------------
    NodoBST* eliminar(NodoBST* nodo, int codigo) {
        // CASO BASE: llegamos a nullptr → el código no estaba en el árbol
        if (nodo == nullptr) return nullptr;

        if (codigo < nodo->dato.codigo) {
            // El código a eliminar está en el subárbol IZQUIERDO
            nodo->izquierdo = eliminar(nodo->izquierdo, codigo);

        } else if (codigo > nodo->dato.codigo) {
            // El código a eliminar está en el subárbol DERECHO
            nodo->derecho = eliminar(nodo->derecho, codigo);

        } else {
            // ¡ENCONTRAMOS el nodo a eliminar! → tres casos:

            // ── CASO 1: nodo HOJA (sin hijos) ──────────────────────
            // Simplemente lo eliminamos y retornamos nullptr para que
            // el padre actualice su puntero a "vacío".
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                delete nodo;
                return nullptr;
            }

            // ── CASO 2a: solo tiene hijo DERECHO ───────────────────
            // El nodo desaparece; su hijo derecho "sube" a ocupar su lugar.
            if (nodo->izquierdo == nullptr) {
                NodoBST* temp = nodo->derecho;   // guardamos el hijo
                delete nodo;                      // eliminamos el nodo actual
                return temp;                      // el hijo sube
            }

            // ── CASO 2b: solo tiene hijo IZQUIERDO ─────────────────
            if (nodo->derecho == nullptr) {
                NodoBST* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }

            // ── CASO 3: tiene DOS hijos ─────────────────────────────
            // No podemos simplemente borrar el nodo porque rompería
            // la cadena. La solución:
            //   1. Buscar el SUCESOR IN-ORDER = el nodo con el código
            //      más pequeño del subárbol DERECHO (mínimo del derecho).
            //   2. Copiar los datos del sucesor a este nodo.
            //   3. Eliminar el sucesor del subárbol derecho (ahora sí
            //      es fácil porque el sucesor tiene a lo sumo un hijo).
            //
            // ¿Por qué el mínimo del subárbol derecho?
            //   Porque es el número más cercano (por arriba) al nodo
            //   que borramos, y reemplazarlo mantiene la invariante BST.

            NodoBST* sucesor = minimo(nodo->derecho);  // paso 1

            // Paso 2: copiar datos del sucesor al nodo actual
            nodo->dato = sucesor->dato;

            // Paso 3: eliminar el sucesor del subárbol derecho
            nodo->derecho = eliminar(nodo->derecho, sucesor->dato.codigo);
        }

        return nodo;   // retornamos el nodo actualizado
    }

    // ----------------------------------------------------------
    // ALTURA (versión recursiva interna)
    // Altura = longitud del camino más largo desde este nodo a una hoja.
    // Árbol vacío → -1 por convención.
    // ----------------------------------------------------------
    int altura(NodoBST* nodo) {
        if (nodo == nullptr) return -1;

        int altIzq = altura(nodo->izquierdo);   // altura del subárbol izquierdo
        int altDer = altura(nodo->derecho);     // altura del subárbol derecho

        // La altura de este nodo es 1 + el mayor de los dos subárboles
        int mayor = (altIzq > altDer) ? altIzq : altDer;
        return 1 + mayor;
    }

    // ----------------------------------------------------------
    // IN-ORDER: izquierdo → raíz → derecho
    // Produce la lista ordenada ascendentemente por código.
    // ¿Por qué? Porque todos los nodos izquierdos < raíz < derechos.
    // ----------------------------------------------------------
    void inOrder(NodoBST* nodo, std::vector<Estudiante>& resultado) {
        if (nodo == nullptr) return;

        inOrder(nodo->izquierdo, resultado);   // 1. visitar todos los menores
        resultado.push_back(nodo->dato);        // 2. visitar este nodo
        inOrder(nodo->derecho, resultado);     // 3. visitar todos los mayores
    }

    // ----------------------------------------------------------
    // PRE-ORDER: raíz → izquierdo → derecho
    // Útil para copiar/serializar el árbol (el padre siempre antes que los hijos).
    // ----------------------------------------------------------
    void preOrder(NodoBST* nodo, std::vector<Estudiante>& resultado) {
        if (nodo == nullptr) return;

        resultado.push_back(nodo->dato);        // 1. visitar este nodo primero
        preOrder(nodo->izquierdo, resultado);  // 2. subárbol izquierdo
        preOrder(nodo->derecho, resultado);    // 3. subárbol derecho
    }

    // ----------------------------------------------------------
    // POST-ORDER: izquierdo → derecho → raíz
    // Útil para liberar memoria (hijos antes que el padre).
    // ----------------------------------------------------------
    void postOrder(NodoBST* nodo, std::vector<Estudiante>& resultado) {
        if (nodo == nullptr) return;

        postOrder(nodo->izquierdo, resultado); // 1. subárbol izquierdo
        postOrder(nodo->derecho, resultado);   // 2. subárbol derecho
        resultado.push_back(nodo->dato);        // 3. visitar este nodo al final
    }

    // ----------------------------------------------------------
    // BFS (Breadth-First Search / por niveles)
    // Usa una COLA manual: array de punteros + índices frente/fin.
    // Visitamos nivel a nivel, de izquierda a derecha.
    // ----------------------------------------------------------
    void bfs(std::vector<Estudiante>& resultado) {
        if (raiz == nullptr) return;

        // Cola manual: arreglo de punteros a nodo
        // Tamaño máximo posible = número de nodos en el árbol.
        // Usamos un arreglo grande por simplicidad.
        const int MAX_COLA = 100000;
        NodoBST* cola[MAX_COLA];
        int frente = 0;   // índice del primer elemento
        int fin    = 0;   // índice donde se insertará el próximo

        // PASO 1: encolar la raíz
        cola[fin] = raiz;
        fin++;

        // PASO 2: mientras la cola no esté vacía...
        while (frente < fin) {
            // Desencolar el primer elemento
            NodoBST* actual = cola[frente];
            frente++;

            // Visitar el nodo actual
            resultado.push_back(actual->dato);

            // Encolar los hijos (si existen) para el siguiente nivel
            if (actual->izquierdo != nullptr) {
                cola[fin] = actual->izquierdo;
                fin++;
            }
            if (actual->derecho != nullptr) {
                cola[fin] = actual->derecho;
                fin++;
            }
        }
    }

    // ----------------------------------------------------------
    // IMPRIMIR ÁRBOL en ASCII (rotado 90° — raíz a la izquierda)
    // ----------------------------------------------------------
    void imprimirArbol(NodoBST* nodo, std::string prefijo, bool esIzquierdo) {
        if (nodo == nullptr) return;

        // Conector visual: ├── para hijo izquierdo, └── para derecho
        std::string conector = esIzquierdo ? "+-- " : "`-- ";
        std::cout << prefijo << conector
                  << nodo->dato.codigo
                  << " [PPA:" << std::fixed << std::setprecision(1)
                  << nodo->dato.ppa << "]\n";

        // Extensión del prefijo para los hijos
        std::string extension = esIzquierdo ? "|   " : "    ";

        // Primero izquierdo (hijo con código menor)
        imprimirArbol(nodo->izquierdo, prefijo + extension, true);
        // Luego derecho (hijo con código mayor)
        imprimirArbol(nodo->derecho,   prefijo + extension, false);
    }

    // ----------------------------------------------------------
    // BUSCAR POR RANGO DE CÓDIGO
    // Aprovecha la invariante BST para descartar subárboles completos:
    //   - Si nodo->codigo < codMin → todo el subárbol izquierdo es
    //     también menor → no buscamos allí.
    //   - Si nodo->codigo > codMax → todo el subárbol derecho es
    //     también mayor → no buscamos allí.
    // Esto es más eficiente que filtrar toda la lista in-order.
    // ----------------------------------------------------------
    void buscarRangoCodigo(NodoBST* nodo, int codMin, int codMax,
                           std::vector<Estudiante>& resultado) {
        if (nodo == nullptr) return;

        // Si el código actual es mayor que el mínimo buscado,
        // puede haber candidatos en el subárbol IZQUIERDO
        if (nodo->dato.codigo > codMin) {
            buscarRangoCodigo(nodo->izquierdo, codMin, codMax, resultado);
        }

        // Si el código actual está dentro del rango, lo incluimos
        if (nodo->dato.codigo >= codMin && nodo->dato.codigo <= codMax) {
            resultado.push_back(nodo->dato);
        }

        // Si el código actual es menor que el máximo buscado,
        // puede haber candidatos en el subárbol DERECHO
        if (nodo->dato.codigo < codMax) {
            buscarRangoCodigo(nodo->derecho, codMin, codMax, resultado);
        }
    }

public:
    // ── Constructor ─────────────────────────────────────────
    ArbolAcademico() : raiz(nullptr) {}

    // ── Destructor: libera toda la memoria del árbol ────────
    ~ArbolAcademico() {
        liberarMemoria(raiz);
    }

    // ── INTERFAZ PÚBLICA ────────────────────────────────────

    NodoBST* obtenerRaiz() const { return raiz; }

    bool estaVacio() { return raiz == nullptr; }

    // Insertar un estudiante en el árbol
    void insertar(Estudiante e) {
        raiz = insertar(raiz, e);
    }

    // Buscar por código → true si existe (y llena el parámetro 'encontrado')
    bool buscar(int codigo, Estudiante& encontrado) {
        NodoBST* nodo = buscar(raiz, codigo);
        if (nodo == nullptr) return false;
        encontrado = nodo->dato;
        return true;
    }

    // Eliminar por código → true si se eliminó, false si no existía
    bool eliminar(int codigo) {
        Estudiante dummy;
        if (!buscar(codigo, dummy)) return false;   // verificar que existe
        raiz = eliminar(raiz, codigo);
        return true;
    }

    // Altura del árbol
    int altura() { return altura(raiz); }

    // ── RECORRIDOS ──────────────────────────────────────────

    std::vector<Estudiante> inOrder() {
        std::vector<Estudiante> res;
        inOrder(raiz, res);
        return res;
    }

    std::vector<Estudiante> preOrder() {
        std::vector<Estudiante> res;
        preOrder(raiz, res);
        return res;
    }

    std::vector<Estudiante> postOrder() {
        std::vector<Estudiante> res;
        postOrder(raiz, res);
        return res;
    }

    std::vector<Estudiante> bfs() {
        std::vector<Estudiante> res;
        bfs(res);
        return res;
    }

    // ── CONSULTAS ───────────────────────────────────────────

    // Filtrar por rango de PPA (recorre in-order y filtra)
    std::vector<Estudiante> porRangoPPA(float ppaMin, float ppaMax = 20.0f) {
        std::vector<Estudiante> todos = inOrder();
        std::vector<Estudiante> res;
        for (int i = 0; i < (int)todos.size(); i++) {
            if (todos[i].ppa >= ppaMin && todos[i].ppa <= ppaMax) {
                res.push_back(todos[i]);
            }
        }
        return res;
    }

    // Filtrar por escuela
    std::vector<Estudiante> porEscuela(std::string escuela) {
        std::vector<Estudiante> todos = inOrder();
        std::vector<Estudiante> res;
        for (int i = 0; i < (int)todos.size(); i++) {
            if (todos[i].escuela == escuela) {
                res.push_back(todos[i]);
            }
        }
        return res;
    }

    // Filtrar por estado académico
    std::vector<Estudiante> porEstado(EstadoAcademico estado) {
        std::vector<Estudiante> todos = inOrder();
        std::vector<Estudiante> res;
        for (int i = 0; i < (int)todos.size(); i++) {
            if (todos[i].estado == estado) {
                res.push_back(todos[i]);
            }
        }
        return res;
    }

    // Estudiante con código máximo (el más a la derecha del árbol)
    bool maximo(Estudiante& resultado) {
        if (raiz == nullptr) return false;
        resultado = maximo(raiz)->dato;
        return true;
    }

    // Estudiante con código mínimo (el más a la izquierda del árbol)
    bool minimo(Estudiante& resultado) {
        if (raiz == nullptr) return false;
        resultado = minimo(raiz)->dato;
        return true;
    }

    // Búsqueda por rango de código (aprovecha la propiedad BST)
    std::vector<Estudiante> buscarRangoCodigo(int codMin, int codMax) {
        std::vector<Estudiante> res;
        buscarRangoCodigo(raiz, codMin, codMax, res);
        return res;
    }

    // ── ESTADÍSTICAS ────────────────────────────────────────
    void estadisticas() {
        std::vector<Estudiante> todos = inOrder();
        int n = todos.size();
        if (n == 0) { std::cout << "Arbol vacio\n"; return; }

        float suma = 0.0f, ppaMin = 20.0f, ppaMax = 0.0f;
        int activos = 0;

        for (int i = 0; i < n; i++) {
            suma += todos[i].ppa;
            if (todos[i].ppa < ppaMin) ppaMin = todos[i].ppa;
            if (todos[i].ppa > ppaMax) ppaMax = todos[i].ppa;
            if (todos[i].estado == ACTIVO) activos++;
        }

        std::cout << std::fixed << std::setprecision(2)
                  << "  total_nodos   : " << n           << "\n"
                  << "  altura        : " << altura()    << "\n"
                  << "  ppa_promedio  : " << suma / n    << "\n"
                  << "  ppa_minimo    : " << ppaMin      << "\n"
                  << "  ppa_maximo    : " << ppaMax      << "\n"
                  << "  total_activos : " << activos     << "\n";
    }

    // ── IMPRESIÓN ASCII ─────────────────────────────────────
    void imprimirArbol() {
        std::cout << "\n-- Estructura del BST --\n";
        if (raiz == nullptr) { std::cout << "(arbol vacio)\n"; return; }
        imprimirArbol(raiz, "", false);
    }
};


// ----------------------------------------------------------
// PRACTICA 10 - Verificadores
// ----------------------------------------------------------
int contar(const NodoBST* nodo){
    if(nodo==nullptr) return 0;
    return 1+contar(nodo->izquierdo)+contar(nodo->derecho);
}

bool esBST(const NodoBST* nodo,int minimo=INT_MIN,int maximo=INT_MAX){
    if(nodo==nullptr) return true;
    if(nodo->dato.codigo<=minimo || nodo->dato.codigo>=maximo) return false;
    return esBST(nodo->izquierdo,minimo,nodo->dato.codigo)
        && esBST(nodo->derecho,nodo->dato.codigo,maximo);
}

bool estaOrdenado(const std::vector<Estudiante>& datos){
    for(size_t i=1;i<datos.size();++i){
        if(datos[i-1].codigo>datos[i].codigo) return false;
    }
    return true;
}

// ════════════════════════════════════════════════════════════
// 5.  GENERADOR DE DATOS para el benchmark
// ════════════════════════════════════════════════════════════
/*
 * Genera n estudiantes con códigos ÚNICOS y ALEATORIOS.
 * Usamos el algoritmo de Fisher-Yates (shuffle manual)
 * para mezclar los códigos y evitar el árbol degenerado.
 */
std::vector<Estudiante> generarDatos(int n, unsigned int semilla = 42) {
    // Paso 1: crear rango de códigos posibles
    // Para n <= 1 000 000 usamos un subconjunto para no gastar tanta memoria
    std::vector<int> pool;
    pool.reserve(n * 3);   // generar más de los necesarios para luego filtrar

    // Generador de números pseudoaleatorios (LCG simple — sin STL random)
    unsigned long long estado = semilla;
    auto siguiente = [&]() -> int {
        // Congruencial lineal: fórmula estándar
        estado = estado * 6364136223846793005ULL + 1442695040888963407ULL;
        return (int)((estado >> 33) % 10000000) + 20000000;  // [20M, 29M]
    };

    // Generar códigos únicos usando un set manual (ya que no queremos STL set)
    // Para simplicidad, usamos vector y búsqueda lineal (válido para el benchmark)
    std::vector<int> codigos;
    codigos.reserve(n);

    while ((int)codigos.size() < n) {
        int c = siguiente();
        // Verificar que no esté duplicado (O(n²) pero solo en el setup del benchmark)
        bool duplicado = false;
        for (int i = 0; i < (int)codigos.size(); i++) {
            if (codigos[i] == c) { duplicado = true; break; }
        }
        if (!duplicado) codigos.push_back(c);
    }

    // Shuffle manual (Fisher-Yates) para mezclar los códigos
    for (int i = n - 1; i > 0; i--) {
        estado = estado * 6364136223846793005ULL + 1442695040888963407ULL;
        int j = (estado >> 33) % (i + 1);
        int tmp = codigos[i]; codigos[i] = codigos[j]; codigos[j] = tmp;
    }

    // Construir los estudiantes
    std::vector<Estudiante> datos;
    datos.reserve(n);
    for (int i = 0; i < n; i++) {
        float ppa = 8.0f + (float)(codigos[i] % 120) / 10.0f;
        if (ppa > 20.0f) ppa = 20.0f;
        datos.push_back(Estudiante(
            codigos[i],
            "Estudiante_" + std::to_string(i),
            "Ingenieria",
            ppa,
            100,
            ACTIVO,
            "2024-I"
        ));
    }
    return datos;
}

// ════════════════════════════════════════════════════════════
// 6.  BENCHMARK
// ════════════════════════════════════════════════════════════
void benchmark(int n) {
    // Para n grande, la generación de datos con verificación de duplicados
    // es O(n²), así que limitamos a 10 000 en el generador manual.
    // Para 100 000 usamos un enfoque diferente: códigos secuenciales mezclados.
    std::vector<Estudiante> datos;

    if (n <= 10000) {
        datos = generarDatos(n);
    } else {
        // Para n grande: generar n códigos secuenciales y mezclar con LCG
        unsigned long long estado = 42;
        std::vector<int> codigos(n);
        for (int i = 0; i < n; i++) codigos[i] = 20000000 + i;
        // Shuffle
        for (int i = n - 1; i > 0; i--) {
            estado = estado * 6364136223846793005ULL + 1442695040888963407ULL;
            int j = (estado >> 33) % (i + 1);
            int tmp = codigos[i]; codigos[i] = codigos[j]; codigos[j] = tmp;
        }
        datos.reserve(n);
        for (int i = 0; i < n; i++) {
            float ppa = 8.0f + (float)(i % 120) / 10.0f;
            if (ppa > 20.0f) ppa = 20.0f;
            datos.push_back(Estudiante(codigos[i], "Est_" + std::to_string(i),
                                       "Ing", ppa, 100, ACTIVO, "2024-I"));
        }
    }

    int codigoBuscar = datos[n / 2].codigo;

    // Medir inserción
    ArbolAcademico arbol;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) arbol.insertar(datos[i]);
    auto t1 = std::chrono::high_resolution_clock::now();
    double msIns = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Medir búsqueda
    Estudiante dummy;
    auto t2 = std::chrono::high_resolution_clock::now();
    arbol.buscar(codigoBuscar, dummy);
    auto t3 = std::chrono::high_resolution_clock::now();
    double msBus = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::cout << std::setw(8) << n
              << "  |  ins: " << std::fixed << std::setprecision(3)
              << std::setw(9) << msIns << " ms"
              << "  |  bus: " << std::setw(8) << msBus << " ms"
              << "  |  altura: " << arbol.altura() << "\n";
}

// ════════════════════════════════════════════════════════════
// 7.  MAIN — programa principal
// ════════════════════════════════════════════════════════════
int main() {
    std::cout << "=== BST Sistema Academico UNA-PUNO (C++17) ===\n\n";

    // ── ACTIVIDADES 8-12: Datos de prueba (los mismos 7 de Python) ──────
    ArbolAcademico arbol;

    arbol.insertar(Estudiante(20210500, "Mamani Quispe, Juan",   "Ing. Sistemas", 15.8f, 120, ACTIVO,     "2021-I"));
    arbol.insertar(Estudiante(20210300, "Huanca Apaza, Maria",   "Ing. Civil",    14.2f, 110, ACTIVO,     "2021-I"));
    arbol.insertar(Estudiante(20210700, "Condori Flores, Pedro", "Medicina",      17.1f, 130, ACTIVO,     "2021-I"));
    arbol.insertar(Estudiante(20210100, "Ticona Lupaca, Rosa",   "Contabilidad",  12.0f,  90, SUSPENDIDO, "2021-I"));
    arbol.insertar(Estudiante(20210400, "Larico Ccama, Carlos",  "Ing. Sistemas", 16.5f, 115, ACTIVO,     "2021-I"));
    arbol.insertar(Estudiante(20210600, "Cutipa Vargas, Elena",  "Agronomia",     13.7f, 100, ACTIVO,     "2021-I"));
    arbol.insertar(Estudiante(20210900, "Pari Choque, Luis",     "Ing. Sistemas", 18.3f, 140, EGRESADO,   "2021-I"));

    // ===== PRACTICA 10: Verificacion de propiedades =====
    assert(estaOrdenado(arbol.inOrder()));
    std::cout << "\nP1 VERIFICADA: InOrder ordenado.\n";

    assert(contar(arbol.obtenerRaiz()) == (int)arbol.inOrder().size());
    std::cout << "P3 VERIFICADA: Conteo correcto de nodos.\n";

    assert(esBST(arbol.obtenerRaiz()));
    std::cout << "P4 VERIFICADA: Se mantiene la propiedad BST.\n\n";


    // ── Estructura ASCII ─────────────────────────────────────────────────
    arbol.imprimirArbol();

    // ── In-Order ─────────────────────────────────────────────────────────
    std::cout << "\n-- In-Order (ordenado por codigo) --\n";
    std::cout << std::left
              << std::setw(12) << "CODIGO"
              << std::setw(35) << "NOMBRE"
              << std::setw(20) << "ESCUELA"
              << "PPA\n";
    std::cout << std::string(72, '-') << "\n";
    for (auto& e : arbol.inOrder()) e.imprimir();

    // ── Pre-Order ────────────────────────────────────────────────────────
    std::cout << "\n-- Pre-Order (codigos) --\n";
    for (auto& e : arbol.preOrder()) std::cout << e.codigo << " ";
    std::cout << "\n";

    // ── Post-Order ───────────────────────────────────────────────────────
    std::cout << "\n-- Post-Order (codigos) --\n";
    for (auto& e : arbol.postOrder()) std::cout << e.codigo << " ";
    std::cout << "\n";

    // ── BFS ──────────────────────────────────────────────────────────────
    std::cout << "\n-- BFS / nivel por nivel (codigos) --\n";
    for (auto& e : arbol.bfs()) std::cout << e.codigo << " ";
    std::cout << "\n";

    std::cout << "\nAltura del arbol: " << arbol.altura() << "\n";

    // ── Búsqueda ─────────────────────────────────────────────────────────
    std::cout << "\n-- Busqueda --\n";
    Estudiante encontrado;
    if (arbol.buscar(20210700, encontrado)) {
        std::cout << "Encontrado: ";
        encontrado.imprimir();
    }
    if (!arbol.buscar(99999999, encontrado)) {
        std::cout << "Buscar 99999999: no encontrado\n";
    }

    // ── Consulta PPA >= 15.0 ─────────────────────────────────────────────
    std::cout << "\n-- Estudiantes con PPA >= 15.0 --\n";
    for (auto& e : arbol.porRangoPPA(15.0f)) e.imprimir();

    // ── Máximo y Mínimo ──────────────────────────────────────────────────
    std::cout << "\n-- Minimo y Maximo (por codigo) --\n";
    Estudiante mi, ma;
    arbol.minimo(mi); std::cout << "Minimo: "; mi.imprimir();
    arbol.maximo(ma); std::cout << "Maximo: "; ma.imprimir();

    // ── Rango de código ──────────────────────────────────────────────────
    std::cout << "\n-- Rango de codigos [20210300, 20210600] --\n";
    for (auto& e : arbol.buscarRangoCodigo(20210300, 20210600)) e.imprimir();

    // ── Estadísticas ─────────────────────────────────────────────────────
    std::cout << "\n-- Estadisticas --\n";
    arbol.estadisticas();

    // ── Eliminación ──────────────────────────────────────────────────────
    std::cout << "\n-- Eliminando codigo 20210300 (nodo con dos hijos) --\n";
    arbol.eliminar(20210300);
    std::cout << "Nodos restantes: " << arbol.inOrder().size() << "\n";
    std::cout << "In-Order tras eliminar: ";
    for (auto& e : arbol.inOrder()) std::cout << e.codigo << " ";
    std::cout << "\n";
    arbol.imprimirArbol();

    // ── TABLA DE VERIFICACIÓN (Actividad 12) ─────────────────────────────
    std::cout << "\n-- Tabla de verificacion (Actividad 12) --\n";
    auto io = arbol.inOrder();   // tras eliminar 20210300
    // Reconstruimos con los 7 originales para comparar
    ArbolAcademico arbol2;
    arbol2.insertar(Estudiante(20210500,"Mamani Quispe, Juan",   "Ing. Sistemas",15.8f,120,ACTIVO,    "2021-I"));
    arbol2.insertar(Estudiante(20210300,"Huanca Apaza, Maria",   "Ing. Civil",   14.2f,110,ACTIVO,    "2021-I"));
    arbol2.insertar(Estudiante(20210700,"Condori Flores, Pedro", "Medicina",     17.1f,130,ACTIVO,    "2021-I"));
    arbol2.insertar(Estudiante(20210100,"Ticona Lupaca, Rosa",   "Contabilidad", 12.0f, 90,SUSPENDIDO,"2021-I"));
    arbol2.insertar(Estudiante(20210400,"Larico Ccama, Carlos",  "Ing. Sistemas",16.5f,115,ACTIVO,    "2021-I"));
    arbol2.insertar(Estudiante(20210600,"Cutipa Vargas, Elena",  "Agronomia",    13.7f,100,ACTIVO,    "2021-I"));
    arbol2.insertar(Estudiante(20210900,"Pari Choque, Luis",     "Ing. Sistemas",18.3f,140,EGRESADO,  "2021-I"));
    auto io2 = arbol2.inOrder();
    Estudiante res2;
    arbol2.buscar(20210700, res2);

    std::cout << std::left << std::setw(35) << "Verificacion"
              << std::setw(15) << "Esperado"
              << "Resultado C++\n";
    std::cout << std::string(65, '-') << "\n";
    std::cout << std::setw(35) << "In-order: primer codigo"
              << std::setw(15) << "20210100"
              << io2.front().codigo << "\n";
    std::cout << std::setw(35) << "In-order: ultimo codigo"
              << std::setw(15) << "20210900"
              << io2.back().codigo << "\n";
    std::cout << std::setw(35) << "Altura del arbol"
              << std::setw(15) << "2"
              << arbol2.altura() << "\n";
    std::cout << std::setw(35) << "Total nodos"
              << std::setw(15) << "7"
              << io2.size() << "\n";
    std::cout << std::setw(35) << "Buscar 20210700 → PPA"
              << std::setw(15) << "17.1"
              << std::fixed << std::setprecision(1) << res2.ppa << "\n";
    Estudiante dummy2;
    std::cout << std::setw(35) << "Buscar 99999999"
              << std::setw(15) << "no encontrado"
              << (arbol2.buscar(99999999, dummy2) ? "encontrado" : "no encontrado") << "\n";
    arbol2.eliminar(20210300);
    std::cout << std::setw(35) << "Nodos tras eliminar 20210300"
              << std::setw(15) << "6"
              << arbol2.inOrder().size() << "\n";

    // ── ÁRBOL DEGENERADO (Actividad 7, punto 1) ──────────────────────────
    std::cout << "\n-- Arbol degenerado: claves en orden creciente --\n";
    ArbolAcademico degenerado;
    for (int i = 0; i < 7; i++) {
        int cod = 20000000 + i * 100;
        degenerado.insertar(Estudiante(cod, "Est_" + std::to_string(i),
                                       "Ing", 10.0f + i, 100, ACTIVO, "2024-I"));
    }
    std::cout << "Altura (esperado 6, como lista enlazada): "
              << degenerado.altura() << "\n";
    degenerado.imprimirArbol();

    // ── BENCHMARK (Actividad 13) ─────────────────────────────────────────
    std::cout << "\n-- Benchmark experimental (Actividad 13) --\n";
    std::cout << std::setw(8) << "N"
              << "  |  Insercion         |  Busqueda       |  Altura\n";
    std::cout << std::string(65, '-') << "\n";
    for (int n : {100, 1000, 10000, 100000}) {
        benchmark(n);
    }

    std::cout << "\n=== Fin del programa ===\n";
    return 0;
}