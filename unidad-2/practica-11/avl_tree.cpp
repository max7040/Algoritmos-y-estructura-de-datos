/*
 * Practica - Algoritmos y Estructuras de Datos
 * Arbol AVL con registro de rotaciones
 *
 * Compilar:  g++ -std=c++17 -O2 -o avl_tree avl_tree.cpp
 * Ejecutar:  ./avl_tree
 */

#include <iostream>
#include <algorithm>

using namespace std;

struct Node {
    int key, height;
    Node *left, *right;
    explicit Node(int val) : key(val), height(1), left(nullptr), right(nullptr) {}
};

int getHeight(Node* n) { return n ? n->height : 0; }
int getBalance(Node* n) { return n ? getHeight(n->left) - getHeight(n->right) : 0; }

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    cout << "  -> Rotacion simple a la DERECHA en el nodo " << y->key << "\n";
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    cout << "  -> Rotacion simple a la IZQUIERDA en el nodo " << x->key << "\n";
    return y;
}

Node* insert(Node* node, int key) {
    if (!node) return new Node(key);

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node; // claves duplicadas no se insertan
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    // Caso Izquierda-Izquierda
    if (balance > 1 && key < node->left->key) {
        cout << "Desbalance Izquierda-Izquierda en " << node->key << " al insertar " << key << "\n";
        return rightRotate(node);
    }
    // Caso Derecha-Derecha
    if (balance < -1 && key > node->right->key) {
        cout << "Desbalance Derecha-Derecha en " << node->key << " al insertar " << key << "\n";
        return leftRotate(node);
    }
    // Caso Izquierda-Derecha
    if (balance > 1 && key > node->left->key) {
        cout << "Desbalance Izquierda-Derecha en " << node->key << " al insertar " << key << "\n";
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Caso Derecha-Izquierda
    if (balance < -1 && key < node->right->key) {
        cout << "Desbalance Derecha-Izquierda en " << node->key << " al insertar " << key << "\n";
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void preOrder(Node* root) {
    if (root) {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node* root) {
    if (root) {
        inOrder(root->left);
        cout << root->key << " ";
        inOrder(root->right);
    }
}

int treeHeight(Node* root) { return getHeight(root); }

int main() {
    Node* root = nullptr;

    int valores[] = {10, 20, 30, 40, 50, 25, 5, 15, 35, 45, 1, 60, 70, 22, 3};
    int total = sizeof(valores) / sizeof(valores[0]); // 15 elementos

    cout << "=== Insertando " << total << " elementos en el AVL ===\n\n";
    for (int v : valores) {
        cout << "Insertar " << v << ":\n";
        root = insert(root, v);
    }

    cout << "\n=== Recorrido InOrder (debe salir ordenado) ===\n";
    inOrder(root);
    cout << "\n";

    cout << "\n=== Recorrido PreOrder (estructura del arbol) ===\n";
    preOrder(root);
    cout << "\n";

    cout << "\nAltura final del arbol: " << treeHeight(root)
         << " (para " << total << " elementos, log2(" << total << ") ~= 3.9,"
         << " lo que confirma el balance O(log n))\n";

    return 0;
}
