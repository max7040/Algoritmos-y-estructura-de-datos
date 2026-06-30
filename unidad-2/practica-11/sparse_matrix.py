"""
Practica - Algoritmos y Estructuras de Datos
Matriz Dispersa en formato COO (Coordinate List)

Ejecutar: python3 sparse_matrix.py
"""

import random


class SparseMatrix:
    """Representa una matriz dispersa en formato COO usando un diccionario
    {(fila, col): valor} que solo guarda los elementos distintos de cero."""

    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.data = {}  # clave = (fila, col), valor = elemento no nulo

    def add(self, r, c, val):
        if val != 0:
            self.data[(r, c)] = val
        elif (r, c) in self.data:
            del self.data[(r, c)]

    def get(self, r, c):
        return self.data.get((r, c), 0)

    def non_zero_count(self):
        return len(self.data)

    def density(self):
        total = self.rows * self.cols
        return self.non_zero_count() / total if total else 0

    def to_coo_lists(self):
        """Devuelve las 3 listas paralelas del formato COO clasico:
        filas, columnas y valores."""
        filas, cols, vals = [], [], []
        for (r, c), v in sorted(self.data.items()):
            filas.append(r)
            cols.append(c)
            vals.append(v)
        return filas, cols, vals

    def display(self, max_dim=15):
        """Imprime la matriz completa solo si es pequena; si es grande,
        imprime unicamente los elementos no nulos para no saturar la salida."""
        if self.rows <= max_dim and self.cols <= max_dim:
            for r in range(self.rows):
                row = [str(self.get(r, c)) for c in range(self.cols)]
                print(" ".join(row))
        else:
            print(f"Matriz {self.rows}x{self.cols} demasiado grande para "
                  f"mostrarse completa; se listan solo los elementos no nulos:")
            for (r, c), v in sorted(self.data.items()):
                print(f"  ({r}, {c}) -> {v}")

    def memory_estimate_bytes(self):
        """Estimacion aproximada de memoria: COO (3 numeros por elemento no
        nulo) vs matriz densa (rows*cols numeros), asumiendo 8 bytes/numero."""
        coo_bytes = self.non_zero_count() * 3 * 8
        dense_bytes = self.rows * self.cols * 8
        return coo_bytes, dense_bytes


def demo_pequena():
    print("=== Ejemplo basico (4x4) del enunciado ===")
    A = SparseMatrix(4, 4)
    A.add(0, 1, 5)
    A.add(1, 3, 8)
    A.add(2, 2, 3)
    A.display()
    print()


def demo_1000x1000():
    print("=== Matriz dispersa 1000x1000 con 20 valores no nulos (formato COO) ===")
    random.seed(42)  # reproducible
    M = SparseMatrix(1000, 1000)

    posiciones_usadas = set()
    while len(posiciones_usadas) < 20:
        r = random.randint(0, 999)
        c = random.randint(0, 999)
        if (r, c) not in posiciones_usadas:
            posiciones_usadas.add((r, c))
            valor = random.randint(1, 99)
            M.add(r, c, valor)

    M.display()

    filas, cols, vals = M.to_coo_lists()
    print("\nRepresentacion COO (3 listas paralelas):")
    print("filas  =", filas)
    print("cols   =", cols)
    print("valores=", vals)

    print(f"\nElementos no nulos : {M.non_zero_count()}")
    print(f"Densidad           : {M.density():.6%}")

    coo_bytes, dense_bytes = M.memory_estimate_bytes()
    print(f"\nMemoria aproximada en formato COO   : {coo_bytes:,} bytes")
    print(f"Memoria aproximada en formato denso : {dense_bytes:,} bytes")
    print(f"Ahorro de memoria                   : "
          f"{(1 - coo_bytes / dense_bytes):.4%}")


if __name__ == "__main__":
    demo_pequena()
    demo_1000x1000()
