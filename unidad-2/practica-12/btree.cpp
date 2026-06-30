/*
 * Practica - Algoritmos y Estructuras de Datos
 * Arbol B (orden m = 2*T, grado minimo T) con insercion, busqueda y ELIMINACION
 *
 * Compilar:  g++ -std=c++17 -O2 -o btree btree.cpp
 * Ejecutar:  ./btree
 */

#include <iostream>
using namespace std;

const int T = 3; // grado minimo (cada nodo: T-1..2T-1 claves)

class BTreeNode {
public:
    int keys[2 * T - 1];
    BTreeNode* children[2 * T];
    int n;        // numero de claves actuales
    bool leaf;     // verdadero si es hoja

    explicit BTreeNode(bool _leaf) {
        leaf = _leaf;
        n = 0;
        for (int i = 0; i < 2 * T; i++) children[i] = nullptr;
    }

    void traverse() {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf) children[i]->traverse();
            cout << keys[i] << " ";
        }
        if (!leaf) children[i]->traverse();
    }

    BTreeNode* search(int k) {
        int i = 0;
        while (i < n && k > keys[i]) i++;
        if (i < n && keys[i] == k) return this;
        if (leaf) return nullptr;
        return children[i]->search(k);
    }

    void insertNonFull(int k);
    void splitChild(int i, BTreeNode* y);

    // ---- Operaciones de eliminacion ----
    void remove(int k);
    int findKey(int k);                 // indice de la primera clave >= k
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPredecessor(int idx);
    int getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
};

class BTree {
public:
    BTreeNode* root;
    BTree() { root = nullptr; }

    void traverse() {
        if (root) root->traverse();
        cout << endl;
    }

    BTreeNode* search(int k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    void insert(int k);
    void remove(int k);
};

// ---------------- INSERCION ----------------

void BTreeNode::insertNonFull(int k) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    } else {
        while (i >= 0 && keys[i] > k) i--;
        if (children[i + 1]->n == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k) i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->n = T - 1;
    for (int j = 0; j < T - 1; j++) z->keys[j] = y->keys[j + T];
    if (!y->leaf) {
        for (int j = 0; j < T; j++) z->children[j] = y->children[j + T];
    }
    y->n = T - 1;
    for (int j = n; j >= i + 1; j--) children[j + 1] = children[j];
    children[i + 1] = z;
    for (int j = n - 1; j >= i; j--) keys[j + 1] = keys[j];
    keys[i] = y->keys[T - 1];
    n++;
}

void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * T - 1) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0] < k) i++;
            s->children[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
}

// ---------------- ELIMINACION ----------------

int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k) idx++;
    return idx;
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; i++) keys[i - 1] = keys[i];
    n--;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    if (children[idx]->n >= T) {
        int pred = getPredecessor(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    } else if (children[idx + 1]->n >= T) {
        int succ = getSuccessor(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    } else {
        merge(idx);
        children[idx]->remove(k);
    }
}

int BTreeNode::getPredecessor(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf) cur = cur->children[cur->n];
    return cur->keys[cur->n - 1];
}

int BTreeNode::getSuccessor(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->leaf) cur = cur->children[0];
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->n >= T) {
        borrowFromPrev(idx);
    } else if (idx != n && children[idx + 1]->n >= T) {
        borrowFromNext(idx);
    } else {
        if (idx != n) merge(idx);
        else merge(idx - 1);
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];

    for (int i = child->n - 1; i >= 0; i--) child->keys[i + 1] = child->keys[i];
    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--) child->children[i + 1] = child->children[i];
    }

    child->keys[0] = keys[idx - 1];
    if (!child->leaf) child->children[0] = sibling->children[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys[child->n] = keys[idx];

    if (!child->leaf) child->children[child->n + 1] = sibling->children[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; i++) sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; i++) sibling->children[i - 1] = sibling->children[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys[T - 1] = keys[idx];
    for (int i = 0; i < sibling->n; i++) child->keys[i + T] = sibling->keys[i];
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; i++) child->children[i + T] = sibling->children[i];
    }

    for (int i = idx + 1; i < n; i++) keys[i - 1] = keys[i];
    for (int i = idx + 2; i <= n; i++) children[i - 1] = children[i];

    child->n += sibling->n + 1;
    n--;

    delete sibling;
}

void BTreeNode::remove(int k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
        if (leaf) removeFromLeaf(idx);
        else removeFromNonLeaf(idx);
    } else {
        if (leaf) {
            cout << "La clave " << k << " no existe en el arbol.\n";
            return;
        }

        bool lastChild = (idx == n);
        if (children[idx]->n < T) fill(idx);

        if (lastChild && idx > n) children[idx - 1]->remove(k);
        else children[idx]->remove(k);
    }
}

void BTree::remove(int k) {
    if (!root) {
        cout << "El arbol esta vacio.\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode* tmp = root;
        if (root->leaf) root = nullptr;
        else root = root->children[0];
        delete tmp;
    }
}

// ---------------- MAIN ----------------

int main() {
    BTree t;
    int valores[] = {10, 20, 5, 6, 12, 30, 7, 17, 25, 3, 14, 28, 1, 19, 35, 8, 22};
    int total = sizeof(valores) / sizeof(valores[0]); // 17 elementos (15-20)

    cout << "=== Insertando " << total << " elementos en el Arbol B (T=" << T << ") ===\n";
    for (int v : valores) t.insert(v);

    cout << "\nRecorrido In-Order (debe salir ordenado, confirma el balance): ";
    t.traverse();

    int k = 6;
    cout << "\nBuscar " << k << ": "
         << (t.search(k) != nullptr ? "Encontrado" : "No encontrado") << "\n";

    cout << "\n=== Eliminando elementos: 6, 17, 30, 1 ===\n";
    int aEliminar[] = {6, 17, 30, 1};
    for (int v : aEliminar) {
        cout << "Eliminar " << v << " -> ";
        t.remove(v);
        cout << "In-Order: ";
        t.traverse();
    }

    cout << "\nBuscar 6 despues de eliminarlo: "
         << (t.search(6) != nullptr ? "Encontrado" : "No encontrado") << "\n";

    return 0;
}
