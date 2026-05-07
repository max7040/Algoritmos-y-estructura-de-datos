import random
import time

def generar_matriz(filas, columnas):
    return [[random.randint(0, 100) for _ in range(columnas)] for _ in range(filas)]

def contar_pares(matriz):
    conteo = 0
    for fila in matriz:
        for valor in fila:
            if valor % 2 == 0:
                conteo += 1
    return conteo

# tamaños a evaluar
tamanos = [(50,50), (100,100), (200,200)]

for filas, columnas in tamanos:
    matriz = generar_matriz(filas, columnas)

    inicio = time.time()
    resultado = contar_pares(matriz)
    fin = time.time()

    print(f"Matriz: {filas}x{columnas}")
    print(f"Números pares: {resultado}")
    print(f"Tiempo de ejecución: {fin - inicio:.6f} segundos")
    print("-" * 40)