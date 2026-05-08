class Nodo:
    def __init__(self, dato):
        self.dato = dato
        self.siguiente = None


class ListaEnlazada:
    def __init__(self):
        self.cabeza = None

    def insertar_inicio(self, dato):
        nuevo = Nodo(dato)
        nuevo.siguiente = self.cabeza
        self.cabeza = nuevo

    def insertar_final(self, dato):
        nuevo = Nodo(dato)

        if not self.cabeza:
            self.cabeza = nuevo
            return

        actual = self.cabeza
        while actual.siguiente:
            actual = actual.siguiente

        actual.siguiente = nuevo

    def eliminar_inicio(self):
        if self.cabeza:
            self.cabeza = self.cabeza.siguiente

    def mostrar(self):
        actual = self.cabeza
        while actual:
            print(actual.dato, end=" -> ")
            actual = actual.siguiente
        print("None")


lista = ListaEnlazada()

lista.insertar_inicio(8)
lista.insertar_inicio(4)
lista.insertar_final(11)

print("Lista inicial:")
lista.mostrar()

lista.eliminar_inicio()

print("Lista final:")
lista.mostrar()