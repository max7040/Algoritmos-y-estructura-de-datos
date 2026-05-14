import random
import time

# ======================= MÉTRICAS =======================
class Metrics:
    def __init__(self):
        self.comparisons = 0
        self.swaps = 0

# ======================= BUBBLE SORT =======================
def bubble_sort(arr, m):
    n = len(arr)
    for i in range(n):
        swapped = False
        for j in range(0, n - i - 1):
            m.comparisons += 1
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                m.swaps += 1
                swapped = True
        if not swapped:
            break

# ======================= INSERTION SORT =======================
def insertion_sort(arr, m):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0:
            m.comparisons += 1
            if arr[j] > key:
                arr[j + 1] = arr[j]
                m.swaps += 1
                j -= 1
            else:
                break
        arr[j + 1] = key

# ======================= MERGE SORT =======================
def merge(arr, left, mid, right, m):
    L = arr[left:mid+1]
    R = arr[mid+1:right+1]

    i = j = 0
    k = left

    while i < len(L) and j < len(R):
        m.comparisons += 1
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        m.swaps += 1
        k += 1

    while i < len(L):
        arr[k] = L[i]
        i += 1
        k += 1
        m.swaps += 1

    while j < len(R):
        arr[k] = R[j]
        j += 1
        k += 1
        m.swaps += 1

def merge_sort_rec(arr, left, right, m):
    if left < right:
        mid = (left + right) // 2
        merge_sort_rec(arr, left, mid, m)
        merge_sort_rec(arr, mid + 1, right, m)
        merge(arr, left, mid, right, m)

def merge_sort(arr, m):
    merge_sort_rec(arr, 0, len(arr) - 1, m)

# ======================= QUICK SORT =======================
def partition(arr, low, high, m):
    pivot = arr[high]
    i = low - 1

    for j in range(low, high):
        m.comparisons += 1
        if arr[j] < pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
            m.swaps += 1

    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    m.swaps += 1
    return i + 1

def quick_sort_rec(arr, low, high, m):
    if low < high:
        pi = partition(arr, low, high, m)
        quick_sort_rec(arr, low, pi - 1, m)
        quick_sort_rec(arr, pi + 1, high, m)

def quick_sort(arr, m):
    quick_sort_rec(arr, 0, len(arr) - 1, m)

# ======================= TEST =======================
def test(data, func, name):
    arr = data.copy()
    m = Metrics()

    start = time.time()
    func(arr, m)
    end = time.time()

    print(f"{name} | Tiempo: {(end - start)*1e6:.2f} μs"
          f" | Comparaciones: {m.comparisons}"
          f" | Intercambios: {m.swaps}")

# ======================= MAIN =======================
n = 1000

# Datos aleatorios
random_data = [random.randint(0, 10000) for _ in range(n)]

# Datos ordenados
sorted_data = sorted(random_data)

# Datos inversos
reverse_data = sorted_data[::-1]

print("===== DATOS ALEATORIOS =====")
test(random_data, bubble_sort,    "Bubble Sort   ")
test(random_data, insertion_sort, "Insertion Sort")
test(random_data, merge_sort,     "Merge Sort    ")
test(random_data, quick_sort,     "Quick Sort    ")

print("\n===== DATOS ORDENADOS =====")
test(sorted_data, bubble_sort,    "Bubble Sort   ")
test(sorted_data, insertion_sort, "Insertion Sort")
test(sorted_data, merge_sort,     "Merge Sort    ")
test(sorted_data, quick_sort,     "Quick Sort    ")

print("\n===== DATOS INVERSOS =====")
test(reverse_data, bubble_sort,    "Bubble Sort   ")
test(reverse_data, insertion_sort, "Insertion Sort")
test(reverse_data, merge_sort,     "Merge Sort    ")
test(reverse_data, quick_sort,     "Quick Sort    ")