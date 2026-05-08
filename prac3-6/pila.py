class Pila:
    def __init__(self):
        self.items = []

    def push(self, dato):
        self.items.append(dato)

    def pop(self):
        if self.items:
            return self.items.pop()
        return "Pila vacía"

    def top(self):
        if self.items:
            return self.items[-1]
        return "Pila vacía"

    def mostrar(self):
        print(self.items[::-1])

pila = Pila()

for x in [5,10,15,20,25]:
    pila.push(x)

print("Pila inicial:")
pila.mostrar()

print("Elemento eliminado:", pila.pop())
print("Elemento eliminado:", pila.pop())

print("Tope actual:", pila.top())

print("Pila final:")
pila.mostrar()